/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 *
 * Bus Glue for pxa27x
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * Modified for LH7A404 from ohci-sa1111.c
 *  by Durgesh Pattamatta <pattamattad@sharpsec.com>
 *
 * Modified for pxa27x from ohci-lh7a404.c
 *  by Nick Bane <nick@cecomputing.co.uk> 26-8-2004
 *
 * This file is licenced under the GPL.
 */

#include <linux/device.h>
#include <asm/mach-types.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>



#if defined(CONFIG_DPM) && defined(CONFIG_MACH_MAINSTONE)
#include <asm/dpm.h>

static struct constraints pxa_hcd_constraints = {
	.count =  1,
	.param = {{DPM_MD_PPLL_ON, 1, 1},},
};
#endif /* CONFIG_DPM && CONFIG_MACH_MAINSTONE */

#define PMM_NPS_MODE           1
#define PMM_GLOBAL_MODE        2
#define PMM_PERPORT_MODE       3

#define PXA_UHC_MAX_PORTNUM    3

#define UHCRHPS(x)              __REG2( 0x4C000050, (x)<<2 )

static int pxa27x_ohci_pmm_state;

/*
  PMM_NPS_MODE -- PMM Non-power switching mode
      Ports are powered continuously.

  PMM_GLOBAL_MODE -- PMM global switching mode
      All ports are powered at the same time.

  PMM_PERPORT_MODE -- PMM per port switching mode
      Ports are powered individually.
 */
static int pxa27x_ohci_select_pmm( int mode )
{
	pxa27x_ohci_pmm_state = mode;

	switch ( mode ) {
	case PMM_NPS_MODE:
		UHCRHDA |= RH_A_NPS;
		break; 
	case PMM_GLOBAL_MODE:
		UHCRHDA &= ~(RH_A_NPS & RH_A_PSM);
		break;
	case PMM_PERPORT_MODE:
		UHCRHDA &= ~(RH_A_NPS);
		UHCRHDA |= RH_A_PSM;

		/* Set port power control mask bits, only 3 ports. */
		UHCRHDB |= (0x7<<17);
		break;
	default:
		printk( KERN_ERR
			"Invalid mode %d, set to non-power switch mode.\n", 
			mode );

		pxa27x_ohci_pmm_state = PMM_NPS_MODE;
		UHCRHDA |= RH_A_NPS;
	}

	return 0;
}

/*
  If you select PMM_PERPORT_MODE, you should set the port power
 */
static int pxa27x_ohci_set_port_power( int port )
{
	if ( (pxa27x_ohci_pmm_state==PMM_PERPORT_MODE)
	     && (port>0) && (port<PXA_UHC_MAX_PORTNUM) ) {
		UHCRHPS(port) |= 0x100;
		return 0;
	}
	return -1;
}

/*
  If you select PMM_PERPORT_MODE, you should set the port power
 */
static int pxa27x_ohci_clear_port_power( int port )
{
	if ( (pxa27x_ohci_pmm_state==PMM_PERPORT_MODE) 
	     && (port>0) && (port<PXA_UHC_MAX_PORTNUM) ) {
		UHCRHPS(port) |= 0x200;
		return 0;
	}
	 
	return -1;
}

extern int usb_disabled(void);

/*-------------------------------------------------------------------------*/

static void pxa27x_start_hc(struct platform_device *dev)
{
	pxa_set_cken(CKEN10_USBHOST, 1);

	UHCHR |= UHCHR_FHR;
	udelay(11);
	UHCHR &= ~UHCHR_FHR;

	UHCHR |= UHCHR_FSBIR;
	while (UHCHR & UHCHR_FSBIR)
		cpu_relax();

	/* This could be properly abstracted away through the
	   device data the day more machines are supported and
	   their differences can be figured out correctly. */
	if (machine_is_mainstone()) {
		/* setup Port1 GPIO pin. */
		pxa_gpio_mode( 88 | GPIO_ALT_FN_1_IN);	/* USBHPWR1 */
		pxa_gpio_mode( 89 | GPIO_ALT_FN_2_OUT);	/* USBHPEN1 */

		/* Set the Power Control Polarity Low and Power Sense
		   Polarity Low to active low. Supply power to USB ports. */
		UHCHR = (UHCHR | UHCHR_PCPL | UHCHR_PSPL) &
			~(UHCHR_SSEP1 | UHCHR_SSEP2 | UHCHR_SSEP3 | UHCHR_SSE);
	}

	UHCHR &= ~UHCHR_SSE;

	UHCHIE = (UHCHIE_UPRIE | UHCHIE_RWIE);
}

static void pxa27x_stop_hc(struct platform_device *dev)
{
	UHCHR |= UHCHR_FHR;
	udelay(11);
	UHCHR &= ~UHCHR_FHR;

	UHCCOMS |= 1;
	udelay(10);

	pxa_set_cken(CKEN10_USBHOST, 0);
}


/*-------------------------------------------------------------------------*/

void usb_hcd_pxa27x_remove (struct usb_hcd *, struct platform_device *);

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */


/**
 * usb_hcd_pxa27x_probe - initialize pxa27x-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */
int usb_hcd_pxa27x_probe (const struct hc_driver *driver,
			  struct usb_hcd **hcd_out,
			  struct platform_device *dev)
{
	int retval;
	struct usb_hcd *hcd = 0;

	unsigned int *addr = NULL;

	if (!request_mem_region(dev->resource[0].start,
				dev->resource[0].end
				- dev->resource[0].start + 1, hcd_name)) {
		pr_debug("request_mem_region failed");
		return -EBUSY;
	}

	pxa27x_start_hc(dev);

	/* Select Power Management Mode */
	pxa27x_ohci_select_pmm( PMM_PERPORT_MODE );

	/* If choosing PMM_PERPORT_MODE, we should set the port power before we use it. */
	if (pxa27x_ohci_set_port_power(1) < 0)
		printk(KERN_ERR "Setting port 1 power failed.\n");

	if (pxa27x_ohci_clear_port_power(2) < 0)
		printk(KERN_ERR "Setting port 2 power failed.\n");

	if (pxa27x_ohci_clear_port_power(3) < 0)
		printk(KERN_ERR "Setting port 3 power failed.\n");

	addr = ioremap(dev->resource[0].start,
		       dev->resource[0].end - dev->resource[0].start + 1);
	if (!addr) {
		pr_debug("ioremap failed");
		retval = -ENOMEM;
		goto err1;
	}

	hcd = driver->hcd_alloc ();
	if (hcd == NULL){
		pr_debug ("hcd_alloc failed");
		retval = -ENOMEM;
		goto err1;
	}

	if(dev->resource[1].flags != IORESOURCE_IRQ){
		pr_debug ("resource[1] is not IORESOURCE_IRQ");
		retval = -ENOMEM;
		goto err1;
	}

	hcd->driver = (struct hc_driver *) driver;
	hcd->description = driver->description;
	hcd->irq = dev->resource[1].start;
	hcd->regs = addr;
	hcd->self.controller = &dev->dev;

	retval = hcd_buffer_create (hcd);
	if (retval != 0) {
		pr_debug ("pool alloc fail");
		goto err1;
	}

	retval = request_irq (hcd->irq, usb_hcd_irq, 
#ifdef CONFIG_PREEMPT_HARDIRQS
			0,
#else
			SA_INTERRUPT,
#endif
			hcd->description, hcd);
	if (retval != 0) {
		pr_debug("request_irq(%d) failed with retval %d\n",hcd->irq,retval);
		retval = -EBUSY;
		goto err2;
	}

	pr_debug ("%s (pxa27x) at 0x%p, irq %d",
	     hcd->description, hcd->regs, hcd->irq);

	usb_bus_init (&hcd->self);
	hcd->self.op = &usb_hcd_operations;
	hcd->self.release = &usb_hcd_release;
	hcd->self.hcpriv = (void *) hcd;
	hcd->self.bus_name = "pxa27x";
	hcd->product_desc = "PXA27x OHCI";

	INIT_LIST_HEAD (&hcd->dev_list);

	usb_register_bus (&hcd->self);

	if ((retval = driver->start (hcd)) < 0) {
		usb_hcd_pxa27x_remove(hcd, dev);
		return retval;
	}

	*hcd_out = hcd;
	return 0;

 err2:
	hcd_buffer_destroy (hcd);
 err1:
	kfree(hcd);
	pxa27x_stop_hc(dev);
	release_mem_region(dev->resource[0].start,
				dev->resource[0].end
			   - dev->resource[0].start + 1);
	return retval;
}


/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/**
 * usb_hcd_pxa27x_remove - shutdown processing for pxa27x-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_pxa27x_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */
void usb_hcd_pxa27x_remove (struct usb_hcd *hcd, struct platform_device *dev)
{
	pr_debug ("remove: %s, state %x", hcd->self.bus_name, hcd->state);

	if (in_interrupt ())
		BUG ();

	hcd->state = USB_STATE_QUIESCING;

	pr_debug ("%s: roothub graceful disconnect", hcd->self.bus_name);
	usb_disconnect (&hcd->self.root_hub);

	hcd->driver->stop (hcd);
	hcd->state = USB_STATE_HALT;

	free_irq (hcd->irq, hcd);
	hcd_buffer_destroy (hcd);

	usb_deregister_bus (&hcd->self);

	pxa27x_stop_hc(dev);
	release_mem_region(dev->resource[0].start,
			   dev->resource[0].end - dev->resource[0].start + 1);
}

/*-------------------------------------------------------------------------*/

static int __devinit
ohci_pxa27x_start (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
	int		ret;

	ohci_dbg (ohci, "ohci_pxa27x_start, ohci:%p", ohci);

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	if ((ret = ohci_run (ohci)) < 0) {
		err ("can't start %s", ohci->hcd.self.bus_name);
		ohci_stop (hcd);
		return ret;
	}

	return 0;
}

/*-------------------------------------------------------------------------*/

static const struct hc_driver ohci_pxa27x_hc_driver = {
	.description =		hcd_name,

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_pxa27x_start,
	.stop =			ohci_stop,

	/*
	 * memory lifecycle (except per-request)
	 */
	.hcd_alloc =		ohci_hcd_alloc,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ohci_urb_enqueue,
	.urb_dequeue =		ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
#ifdef  CONFIG_USB_SUSPEND
	.hub_suspend =		ohci_hub_suspend,
	.hub_resume =		ohci_hub_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

/*-------------------------------------------------------------------------*/

static int ohci_hcd_pxa27x_drv_probe(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct usb_hcd *hcd = NULL;
	int ret;

	pr_debug ("In ohci_hcd_pxa27x_drv_probe");

	if (usb_disabled())
		return -ENODEV;

	ret = usb_hcd_pxa27x_probe(&ohci_pxa27x_hc_driver, &hcd, pdev);

	if (ret == 0)
		dev_set_drvdata(dev, hcd);

#if defined(CONFIG_DPM) && defined(CONFIG_MACH_MAINSTONE)
	pdev->dev.constraints=&pxa_hcd_constraints;
	assert_constraints(pdev->dev.constraints);
#endif /* CONFIG_DPM && CONFIG_MACH_MAINSTONE */

	return ret;
}

static int ohci_hcd_pxa27x_drv_remove(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct usb_hcd *hcd = dev_get_drvdata(dev);

	usb_hcd_pxa27x_remove(hcd, pdev);
	dev_set_drvdata(dev, NULL);
	return 0;
}

static int ohci_hcd_pxa27x_drv_suspend(struct device *dev, u32 state, u32 level)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct ohci_hcd *ohci = hcd_to_ohci(dev_get_drvdata(dev));
	int status = -EINVAL;

	if (state <= dev->power.power_state)
		return 0;

	dev_dbg(dev, "suspend to %d\n", state);
	down(&ohci->hcd.self.root_hub->serialize);
	status = ohci_hub_suspend(&ohci->hcd);
	if (status == 0) {
		if (state >= 4) {
			pxa27x_stop_hc(pdev);
			ohci->hcd.self.root_hub->state = USB_STATE_SUSPENDED;
			state = 4;
		}
		ohci->hcd.state = HCD_STATE_SUSPENDED;
		dev->power.power_state = state;
	}
	up(&ohci->hcd.self.root_hub->serialize);
	return status;
}

static int ohci_hcd_pxa27x_drv_resume(struct device *dev, u32 state)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct ohci_hcd *ohci = hcd_to_ohci(dev_get_drvdata(dev));
	int status = 0;

	switch (dev->power.power_state) {
	case 0:
		break;
	case 4:
		if (time_before(jiffies, ohci->next_statechange))
			msleep(5);
		ohci->next_statechange = jiffies;
		/* FALLTHROUGH */
	default:
		dev_dbg(dev, "resume from %d\n", dev->power.power_state);
		pxa27x_start_hc(pdev);
#ifdef  CONFIG_USB_SUSPEND
		/* get extra cleanup even if remote wakeup isn't in use */
		status = usb_resume_device(ohci->hcd.self.root_hub);
#else
		down(&ohci->hcd.self.root_hub->serialize);
		status = ohci_hub_resume(&ohci->hcd);
		up(&ohci->hcd.self.root_hub->serialize);
#endif
		if (status == 0)
			dev->power.power_state = 0;
		break;
	}
	return status;
}


static struct device_driver ohci_hcd_pxa27x_driver = {
	.name		= "pxa27x-ohci",
	.bus		= &platform_bus_type,
	.probe		= ohci_hcd_pxa27x_drv_probe,
	.remove		= ohci_hcd_pxa27x_drv_remove,
	.suspend	= ohci_hcd_pxa27x_drv_suspend, 
	.resume		= ohci_hcd_pxa27x_drv_resume, 
};

static int __init ohci_hcd_pxa27x_init (void)
{
	pr_debug (DRIVER_INFO " (pxa27x)");
	pr_debug ("block sizes: ed %d td %d\n",
		sizeof (struct ed), sizeof (struct td));

	return driver_register(&ohci_hcd_pxa27x_driver);
}

static void __exit ohci_hcd_pxa27x_cleanup (void)
{
	driver_unregister(&ohci_hcd_pxa27x_driver);
}

module_init (ohci_hcd_pxa27x_init);
module_exit (ohci_hcd_pxa27x_cleanup);

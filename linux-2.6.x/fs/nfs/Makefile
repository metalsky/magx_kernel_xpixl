#
# Makefile for the Linux nfs filesystem routines.
#
ifeq ($(CONFIG_ARM),y)
ifeq ($(CONFIG_GCOV_ALL),y)
CFLAGS := $(CFLAGS_NOGCOV)
endif
endif

obj-$(CONFIG_NFS_FS) += nfs.o

nfs-y 			:= dir.o file.o inode.o nfs2xdr.o pagelist.o \
			   proc.o read.o symlink.o unlink.o write.o
nfs-$(CONFIG_ROOT_NFS)	+= nfsroot.o mount_clnt.o      
nfs-$(CONFIG_NFS_V3)	+= nfs3proc.o nfs3xdr.o
nfs-$(CONFIG_NFS_V4)	+= nfs4proc.o nfs4xdr.o nfs4state.o nfs4renewd.o \
			   delegation.o idmap.o \
			   callback.o callback_xdr.o callback_proc.o
nfs-$(CONFIG_NFS_DIRECTIO) += direct.o
nfs-objs		:= $(nfs-y)

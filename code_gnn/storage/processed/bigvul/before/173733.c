static int fuse_setup(struct fuse* fuse, gid_t gid, mode_t mask) {
 char opts[256];

    fuse->fd = open("/dev/fuse", O_RDWR);
 if (fuse->fd == -1) {
        ERROR("failed to open fuse device: %s\n", strerror(errno));
 return -1;
 }

    umount2(fuse->dest_path, MNT_DETACH);

    snprintf(opts, sizeof(opts),
 "fd=%i,rootmode=40000,default_permissions,allow_other,user_id=%d,group_id=%d",
            fuse->fd, fuse->global->uid, fuse->global->gid);
 if (mount("/dev/fuse", fuse->dest_path, "fuse", MS_NOSUID | MS_NODEV | MS_NOEXEC |
            MS_NOATIME, opts) != 0) {
        ERROR("failed to mount fuse filesystem: %s\n", strerror(errno));
 return -1;
 }

    fuse->gid = gid;
    fuse->mask = mask;

 return 0;
}

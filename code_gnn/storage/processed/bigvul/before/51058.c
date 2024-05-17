int ovl_create_real(struct inode *dir, struct dentry *newdentry,
		    struct kstat *stat, const char *link,
		    struct dentry *hardlink, bool debug)
{
	int err;

	if (newdentry->d_inode)
		return -ESTALE;

	if (hardlink) {
		err = ovl_do_link(hardlink, dir, newdentry, debug);
	} else {
		switch (stat->mode & S_IFMT) {
		case S_IFREG:
			err = ovl_do_create(dir, newdentry, stat->mode, debug);
			break;

		case S_IFDIR:
			err = ovl_do_mkdir(dir, newdentry, stat->mode, debug);
			break;

		case S_IFCHR:
		case S_IFBLK:
		case S_IFIFO:
		case S_IFSOCK:
			err = ovl_do_mknod(dir, newdentry,
					   stat->mode, stat->rdev, debug);
			break;

		case S_IFLNK:
			err = ovl_do_symlink(dir, newdentry, link, debug);
			break;

		default:
			err = -EPERM;
		}
	}
	if (!err && WARN_ON(!newdentry->d_inode)) {
		 
		err = -ENOENT;
	}
	return err;
}
static int jffs2_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct jffs2_inode_info *f;
	struct jffs2_sb_info *c;
	struct inode *inode = filp->f_path.dentry->d_inode;
	struct jffs2_full_dirent *fd;
	unsigned long offset, curofs;

	D1(printk(KERN_DEBUG "jffs2_readdir() for dir_i #%lu\n", filp->f_path.dentry->d_inode->i_ino));

	f = JFFS2_INODE_INFO(inode);
	c = JFFS2_SB_INFO(inode->i_sb);

	offset = filp->f_pos;

	if (offset == 0) {
		D1(printk(KERN_DEBUG "Dirent 0: \".\", ino #%lu\n", inode->i_ino));
		if (filldir(dirent, ".", 1, 0, inode->i_ino, DT_DIR) < 0)
			goto out;
		offset++;
	}
	if (offset == 1) {
		unsigned long pino = parent_ino(filp->f_path.dentry);
		D1(printk(KERN_DEBUG "Dirent 1: \"..\", ino #%lu\n", pino));
		if (filldir(dirent, "..", 2, 1, pino, DT_DIR) < 0)
			goto out;
		offset++;
	}

	curofs=1;
	down(&f->sem);
	for (fd = f->dents; fd; fd = fd->next) {

		curofs++;
		 
		if (curofs < offset) {
			D2(printk(KERN_DEBUG "Skipping dirent: \"%s\", ino #%u, type %d, because curofs %ld < offset %ld\n",
				  fd->name, fd->ino, fd->type, curofs, offset));
			continue;
		}
		if (!fd->ino) {
			D2(printk(KERN_DEBUG "Skipping deletion dirent \"%s\"\n", fd->name));
			offset++;
			continue;
		}
		D2(printk(KERN_DEBUG "Dirent %ld: \"%s\", ino #%u, type %d\n", offset, fd->name, fd->ino, fd->type));
		if (filldir(dirent, fd->name, strlen(fd->name), offset, fd->ino, fd->type) < 0)
			break;
		offset++;
	}
	up(&f->sem);
 out:
	filp->f_pos = offset;
	return 0;
}

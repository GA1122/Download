SYSCALL_DEFINE4(osf_getdirentries, unsigned int, fd,
		struct osf_dirent __user *, dirent, unsigned int, count,
		long __user *, basep)
{
	int error;
	struct file *file;
	struct osf_dirent_callback buf;

	error = -EBADF;
	file = fget(fd);
	if (!file)
		goto out;

	buf.dirent = dirent;
	buf.basep = basep;
	buf.count = count;
	buf.error = 0;

	error = vfs_readdir(file, osf_filldir, &buf);
	if (error >= 0)
		error = buf.error;
	if (count != buf.count)
		error = count - buf.count;

	fput(file);
 out:
	return error;
}

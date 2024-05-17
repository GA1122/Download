static struct perf_event *perf_fget_light(int fd, int *fput_needed)
{
	struct file *file;

	file = fget_light(fd, fput_needed);
	if (!file)
		return ERR_PTR(-EBADF);

	if (file->f_op != &perf_fops) {
		fput_light(file, *fput_needed);
		*fput_needed = 0;
		return ERR_PTR(-EBADF);
	}

	return file->private_data;
}

static int lxc_attach_to_ns(pid_t pid, int which)
{
	int fd[LXC_NS_MAX];
	int i, j, saved_errno;


	if (access("/proc/self/ns", X_OK)) {
		ERROR("Does this kernel version support 'attach' ?");
		return -1;
	}

	for (i = 0; i < LXC_NS_MAX; i++) {
		 
		if (which != -1 && !(which & ns_info[i].clone_flag)) {
			fd[i] = -1;
			continue;
		}

		fd[i] = lxc_preserve_ns(pid, ns_info[i].proc_name);
		if (fd[i] < 0) {
			saved_errno = errno;

			 
			for (j = 0; j < i; j++)
				close(fd[j]);

			errno = saved_errno;
			SYSERROR("failed to open namespace: '%s'.", ns_info[i].proc_name);
			return -1;
		}
	}

	for (i = 0; i < LXC_NS_MAX; i++) {
		if (fd[i] < 0)
			continue;

		if (setns(fd[i], 0) < 0) {
			saved_errno = errno;

			for (j = i; j < LXC_NS_MAX; j++)
				close(fd[j]);

			errno = saved_errno;
			SYSERROR("Failed to attach to namespace \"%s\".", ns_info[i].proc_name);
			return -1;
		}

		DEBUG("Attached to namespace \"%s\".", ns_info[i].proc_name);

		close(fd[i]);
	}

	return 0;
}
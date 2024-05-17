void initialise_threads(int fragment_buffer_size, int data_buffer_size)
{
	struct rlimit rlim;
	int i, max_files, res;
	sigset_t sigmask, old_mask;

	 
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGQUIT);
	sigaddset(&sigmask, SIGHUP);
	if(pthread_sigmask(SIG_BLOCK, &sigmask, NULL) != 0)
		EXIT_UNSQUASH("Failed to set signal mask in initialise_threads"
			"\n");

	 
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGINT);
	sigaddset(&sigmask, SIGTERM);
	if(pthread_sigmask(SIG_BLOCK, &sigmask, &old_mask) != 0)
		EXIT_UNSQUASH("Failed to set signal mask in initialise_threads"
			"\n");

	if(processors == -1) {
#ifndef linux
		int mib[2];
		size_t len = sizeof(processors);

		mib[0] = CTL_HW;
#ifdef HW_AVAILCPU
		mib[1] = HW_AVAILCPU;
#else
		mib[1] = HW_NCPU;
#endif

		if(sysctl(mib, 2, &processors, &len, NULL, 0) == -1) {
			ERROR("Failed to get number of available processors.  "
				"Defaulting to 1\n");
			processors = 1;
		}
#else
		processors = sysconf(_SC_NPROCESSORS_ONLN);
#endif
	}

	if(add_overflow(processors, 3) ||
			multiply_overflow(processors + 3, sizeof(pthread_t)))
		EXIT_UNSQUASH("Processors too large\n");

	thread = malloc((3 + processors) * sizeof(pthread_t));
	if(thread == NULL)
		EXIT_UNSQUASH("Out of memory allocating thread descriptors\n");
	inflator_thread = &thread[3];

	 
	res = getrlimit(RLIMIT_NOFILE, &rlim);
	if (res == -1) {
		ERROR("failed to get open file limit!  Defaulting to 1\n");
		rlim.rlim_cur = 1;
	}

	if (rlim.rlim_cur != RLIM_INFINITY) {
		 
		if (rlim.rlim_cur <= OPEN_FILE_MARGIN)
			 
			max_files = 1;
		else
			max_files = rlim.rlim_cur - OPEN_FILE_MARGIN;
	} else
		max_files = -1;

	 
	open_init(max_files);

	 
	if (max_files != -1) {
		if(add_overflow(data_buffer_size, max_files) ||
				add_overflow(data_buffer_size, max_files * 2))
			EXIT_UNSQUASH("Data queue size is too large\n");

		to_reader = queue_init(max_files + data_buffer_size);
		to_inflate = queue_init(max_files + data_buffer_size);
		to_writer = queue_init(max_files * 2 + data_buffer_size);
	} else {
		int all_buffers_size;

		if(add_overflow(fragment_buffer_size, data_buffer_size))
			EXIT_UNSQUASH("Data and fragment queues combined are"
							" too large\n");

		all_buffers_size = fragment_buffer_size + data_buffer_size;

		if(add_overflow(all_buffers_size, all_buffers_size))
			EXIT_UNSQUASH("Data and fragment queues combined are"
							" too large\n");

		to_reader = queue_init(all_buffers_size);
		to_inflate = queue_init(all_buffers_size);
		to_writer = queue_init(all_buffers_size * 2);
	}

	from_writer = queue_init(1);

	fragment_cache = cache_init(block_size, fragment_buffer_size);
	data_cache = cache_init(block_size, data_buffer_size);
	pthread_create(&thread[0], NULL, reader, NULL);
	pthread_create(&thread[1], NULL, writer, NULL);
	pthread_create(&thread[2], NULL, progress_thread, NULL);
	init_info();
	pthread_mutex_init(&fragment_mutex, NULL);

	for(i = 0; i < processors; i++) {
		if(pthread_create(&inflator_thread[i], NULL, inflator, NULL) !=
				 0)
			EXIT_UNSQUASH("Failed to create thread\n");
	}

	printf("Parallel unsquashfs: Using %d processor%s\n", processors,
			processors == 1 ? "" : "s");

	if(pthread_sigmask(SIG_SETMASK, &old_mask, NULL) != 0)
		EXIT_UNSQUASH("Failed to set signal mask in initialise_threads"
			"\n");
}

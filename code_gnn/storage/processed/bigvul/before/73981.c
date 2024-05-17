main(int argc, char *argv[])
{
	int r;

	parse_args(argc, argv);

	establish_signal_handlers();

	r = term_lib_init();
	if ( r < 0 )
		fatal("term_init failed: %s", term_strerror(term_errno, errno));

#ifdef UUCP_LOCK_DIR
	if ( ! opts.nolock ) uucp_lockname(UUCP_LOCK_DIR, opts.port);
	if ( uucp_lock() < 0 )
		fatal("cannot lock %s: %s", opts.port, strerror(errno));
#endif

	tty_fd = open(opts.port, O_RDWR | O_NONBLOCK | O_NOCTTY);
	if (tty_fd < 0)
		fatal("cannot open %s: %s", opts.port, strerror(errno));

#ifdef USE_FLOCK
	if ( ! opts.nolock ) {
		r = flock(tty_fd, LOCK_EX | LOCK_NB);
		if ( r < 0 )
			fatal("cannot lock %s: %s", opts.port, strerror(errno));
	}
#endif

	if ( opts.noinit ) {
		r = term_add(tty_fd);
	} else {
		r = term_set(tty_fd,
					 1,               
					 opts.baud,       
					 opts.parity,     
					 opts.databits,   
					 opts.flow,       
					 1,               
					 !opts.noreset);  
	}
	if ( r < 0 )
		fatal("failed to add device %s: %s", 
			  opts.port, term_strerror(term_errno, errno));
	r = term_apply(tty_fd);
	if ( r < 0 )
		fatal("failed to config device %s: %s", 
			  opts.port, term_strerror(term_errno, errno));

	set_tty_write_sz(term_get_baudrate(tty_fd, NULL));
	
	r = term_add(STI);
	if ( r < 0 )
		fatal("failed to add I/O device: %s", 
			  term_strerror(term_errno, errno));
	term_set_raw(STI);
	r = term_apply(STI);
	if ( r < 0 )
		fatal("failed to set I/O device to raw mode: %s",
			  term_strerror(term_errno, errno));

#ifdef LINENOISE
	init_send_receive_history();
#endif

	fd_printf(STO, "Terminal ready\r\n");
	loop();

#ifdef LINENOISE
	cleanup_send_receive_history();
#endif

	fd_printf(STO, "\r\n");
	if ( opts.noreset ) {
		fd_printf(STO, "Skipping tty reset...\r\n");
		term_erase(tty_fd);
	}

	if ( sig_exit )
		fd_printf(STO, "Picocom was killed\r\n");
	else
		fd_printf(STO, "Thanks for using picocom\r\n");
	 
	sleep(1);

#ifdef UUCP_LOCK_DIR
	uucp_unlock();
#endif

	return EXIT_SUCCESS;
}
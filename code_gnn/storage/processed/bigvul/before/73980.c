loop(void)
{
	enum {
		ST_COMMAND,
		ST_TRANSPARENT
	} state;
	fd_set rdset, wrset;
	int r, n;
	unsigned char c;

	tty_q.len = 0;
	state = ST_TRANSPARENT;

	while ( ! sig_exit ) {
		FD_ZERO(&rdset);
		FD_ZERO(&wrset);
		FD_SET(STI, &rdset);
		FD_SET(tty_fd, &rdset);
		if ( tty_q.len ) FD_SET(tty_fd, &wrset);

		r = select(tty_fd + 1, &rdset, &wrset, NULL, NULL);
		if ( r < 0 )  {
			if ( errno == EINTR )
				continue;
			else
				fatal("select failed: %d : %s", errno, strerror(errno));
		}

		if ( FD_ISSET(STI, &rdset) ) {

			 

			do {
				n = read(STI, &c, 1);
			} while (n < 0 && errno == EINTR);
			if (n == 0) {
				fatal("stdin closed");
			} else if (n < 0) {
				 
				if ( errno != EAGAIN && errno != EWOULDBLOCK ) 
					fatal("read from stdin failed: %s", strerror(errno));
				else
					goto skip_proc_STI;
			}

			switch (state) {
			case ST_COMMAND:
				if ( c == opts.escape ) {
					 
					if (tty_q.len + M_MAXMAP <= TTY_Q_SZ) {
						n = do_map((char *)tty_q.buff + tty_q.len, 
								   opts.omap, c);
						tty_q.len += n;
						if ( opts.lecho ) 
							map_and_write(STO, opts.emap, c);
					} else {
						fd_printf(STO, "\x07");
					}
				} else {
					 
					if ( do_command(c) )
						 
						return;
				}
				state = ST_TRANSPARENT;
				break;
			case ST_TRANSPARENT:
				if ( c == opts.escape ) {
					state = ST_COMMAND;
				} else {
					if (tty_q.len + M_MAXMAP <= TTY_Q_SZ) {
						n = do_map((char *)tty_q.buff + tty_q.len, 
								   opts.omap, c);
						tty_q.len += n;
						if ( opts.lecho ) 
							map_and_write(STO, opts.emap, c);
					} else 
						fd_printf(STO, "\x07");
				}
				break;
			default:
				assert(0);
				break;
			}
		}
	skip_proc_STI:

		if ( FD_ISSET(tty_fd, &rdset) ) {

			 

			do {
				n = read(tty_fd, &c, 1);
			} while (n < 0 && errno == EINTR);
			if (n == 0) {
				fatal("term closed");
			} else if ( n < 0 ) {
				if ( errno != EAGAIN && errno != EWOULDBLOCK )
					fatal("read from term failed: %s", strerror(errno));
			} else {
				map_and_write(STO, opts.imap, c);
			}
		}

		if ( FD_ISSET(tty_fd, &wrset) ) {

			 

			int sz;
			sz = (tty_q.len < tty_write_sz) ? tty_q.len : tty_write_sz;
			do {
				n = write(tty_fd, tty_q.buff, sz);
			} while ( n < 0 && errno == EINTR );
			if ( n <= 0 )
				fatal("write to term failed: %s", strerror(errno));
			memmove(tty_q.buff, tty_q.buff + n, tty_q.len - n);
			tty_q.len -= n;
		}
	}
}
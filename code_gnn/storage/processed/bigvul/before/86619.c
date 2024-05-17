static void fpm_stdio_child_said(struct fpm_event_s *ev, short which, void *arg)  
{
	static const int max_buf_size = 1024;
	int fd = ev->fd;
	char buf[max_buf_size];
	struct fpm_child_s *child;
	int is_stdout;
	struct fpm_event_s *event;
	int fifo_in = 1, fifo_out = 1;
	int is_last_message = 0;
	int in_buf = 0;
	int res;

	if (!arg) {
		return;
	}
	child = (struct fpm_child_s *)arg;
	is_stdout = (fd == child->fd_stdout);
	if (is_stdout) {
		event = &child->ev_stdout;
	} else {
		event = &child->ev_stderr;
	}

	while (fifo_in || fifo_out) {
		if (fifo_in) {
			res = read(fd, buf + in_buf, max_buf_size - 1 - in_buf);
			if (res <= 0) {  
				fifo_in = 0;
				if (res < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
					 
				} else {  

					if (res < 0) {  
						zlog(ZLOG_SYSERROR, "unable to read what child say");
					}

					fpm_event_del(event);
					is_last_message = 1;

					if (is_stdout) {
						close(child->fd_stdout);
						child->fd_stdout = -1;
					} else {
						close(child->fd_stderr);
						child->fd_stderr = -1;
					}
				}
			} else {
				in_buf += res;
			}
		}

		if (fifo_out) {
			if (in_buf == 0) {
				fifo_out = 0;
			} else {
				char *nl;
				int should_print = 0;
				buf[in_buf] = '\0';

				 

				 
				if (in_buf == max_buf_size - 1) {
					should_print = 1;
				}

				 
				if (!fifo_in) {
					should_print = 1;
				}

				nl = strchr(buf, '\n');
				if (nl || should_print) {

					if (nl) {
						*nl = '\0';
					}

					zlog(ZLOG_WARNING, "[pool %s] child %d said into %s: \"%s\"%s", child->wp->config->name,
					  (int) child->pid, is_stdout ? "stdout" : "stderr", buf, is_last_message ? ", pipe is closed" : "");

					if (nl) {
						int out_buf = 1 + nl - buf;
						memmove(buf, buf + out_buf, in_buf - out_buf);
						in_buf -= out_buf;
					} else {
						in_buf = 0;
					}
				}
			}
		}
	}
}
 
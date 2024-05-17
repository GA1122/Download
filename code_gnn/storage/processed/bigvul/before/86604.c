static void fpm_child_close(struct fpm_child_s *child, int in_event_loop)  
{
	if (child->fd_stdout != -1) {
		if (in_event_loop) {
			fpm_event_fire(&child->ev_stdout);
		}
		if (child->fd_stdout != -1) {
			close(child->fd_stdout);
		}
	}

	if (child->fd_stderr != -1) {
		if (in_event_loop) {
			fpm_event_fire(&child->ev_stderr);
		}
		if (child->fd_stderr != -1) {
			close(child->fd_stderr);
		}
	}

	fpm_child_free(child);
}
 
sigend(__attribute__((unused)) void *v, __attribute__((unused)) int sig)
{
	int status;
	int ret;
	int wait_count = 0;
	struct timeval start_time, now;
#ifdef HAVE_SIGNALFD
	struct timeval timeout = {
		.tv_sec = child_wait_time,
		.tv_usec = 0
	};
	int signal_fd = master->signal_fd;
	fd_set read_set;
	struct signalfd_siginfo siginfo;
	sigset_t sigmask;
#else
	sigset_t old_set, child_wait;
	struct timespec timeout = {
		.tv_sec = child_wait_time,
		.tv_nsec = 0
	};
#endif

	 
	thread_add_terminate_event(master);

	log_message(LOG_INFO, "Stopping");

#ifdef HAVE_SIGNALFD
	 
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGCHLD);
	signalfd(signal_fd, &sigmask, 0);
	FD_ZERO(&read_set);
#else
	sigmask_func(0, NULL, &old_set);
	if (!sigismember(&old_set, SIGCHLD)) {
		sigemptyset(&child_wait);
		sigaddset(&child_wait, SIGCHLD);
		sigmask_func(SIG_BLOCK, &child_wait, NULL);
	}
#endif

#ifdef _WITH_VRRP_
	if (vrrp_child > 0) {
		if (kill(vrrp_child, SIGTERM)) {
			 
			if (errno == ESRCH)
				vrrp_child = 0;
		}
		else
			wait_count++;
	}
#endif
#ifdef _WITH_LVS_
	if (checkers_child > 0) {
		if (kill(checkers_child, SIGTERM)) {
			if (errno == ESRCH)
				checkers_child = 0;
		}
		else
			wait_count++;
	}
#endif
#ifdef _WITH_BFD_
	if (bfd_child > 0) {
		if (kill(bfd_child, SIGTERM)) {
			if (errno == ESRCH)
				bfd_child = 0;
		}
		else
			wait_count++;
	}
#endif

	gettimeofday(&start_time, NULL);
	while (wait_count) {
#ifdef HAVE_SIGNALFD
		FD_SET(signal_fd, &read_set);
		ret = select(signal_fd + 1, &read_set, NULL, NULL, &timeout);
		if (ret == 0)
			break;
		if (ret == -1) {
			if (errno == EINTR)
				continue;

			log_message(LOG_INFO, "Terminating select returned errno %d", errno);
			break;
		}

		if (!FD_ISSET(signal_fd, &read_set)) {
			log_message(LOG_INFO, "Terminating select did not return select_fd");
			continue;
		}

		if (read(signal_fd, &siginfo, sizeof(siginfo)) != sizeof(siginfo)) {
			log_message(LOG_INFO, "Terminating signal read did not read entire siginfo");
			break;
		}

		status = siginfo.ssi_code == CLD_EXITED ? W_EXITCODE(siginfo.ssi_status, 0) :
			 siginfo.ssi_code == CLD_KILLED ? W_EXITCODE(0, siginfo.ssi_status) :
							   WCOREFLAG;

#ifdef _WITH_VRRP_
		if (vrrp_child > 0 && vrrp_child == (pid_t)siginfo.ssi_pid) {
			report_child_status(status, vrrp_child, PROG_VRRP);
			vrrp_child = 0;
			wait_count--;
		}
#endif

#ifdef _WITH_LVS_
		if (checkers_child > 0 && checkers_child == (pid_t)siginfo.ssi_pid) {
			report_child_status(status, checkers_child, PROG_CHECK);
			checkers_child = 0;
			wait_count--;
		}
#endif
#ifdef _WITH_BFD_
		if (bfd_child > 0 && bfd_child == (pid_t)siginfo.ssi_pid) {
			report_child_status(status, bfd_child, PROG_BFD);
			bfd_child = 0;
			wait_count--;
		}
#endif

#else
		ret = sigtimedwait(&child_wait, NULL, &timeout);
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			if (errno == EAGAIN)
				break;
		}

#ifdef _WITH_VRRP_
		if (vrrp_child > 0 && vrrp_child == waitpid(vrrp_child, &status, WNOHANG)) {
			report_child_status(status, vrrp_child, PROG_VRRP);
			vrrp_child = 0;
			wait_count--;
		}
#endif

#ifdef _WITH_LVS_
		if (checkers_child > 0 && checkers_child == waitpid(checkers_child, &status, WNOHANG)) {
			report_child_status(status, checkers_child, PROG_CHECK);
			checkers_child = 0;
			wait_count--;
		}
#endif
#ifdef _WITH_BFD_
		if (bfd_child > 0 && bfd_child == waitpid(bfd_child, &status, WNOHANG)) {
			report_child_status(status, bfd_child, PROG_BFD);
			bfd_child = 0;
			wait_count--;
		}
#endif

#endif

		if (wait_count) {
			gettimeofday(&now, NULL);
			timeout.tv_sec = child_wait_time - (now.tv_sec - start_time.tv_sec);
#ifdef HAVE_SIGNALFD
			timeout.tv_usec = (start_time.tv_usec - now.tv_usec);
			if (timeout.tv_usec < 0) {
				timeout.tv_usec += 1000000L;
				timeout.tv_sec--;
			}
#else
			timeout.tv_nsec = (start_time.tv_usec - now.tv_usec) * 1000;
			if (timeout.tv_nsec < 0) {
				timeout.tv_nsec += 1000000000L;
				timeout.tv_sec--;
			}
#endif
			if (timeout.tv_sec < 0)
				break;
		}
	}

	 
#ifdef _WITH_VRRP_
	if (vrrp_child) {
		log_message(LOG_INFO, "vrrp process failed to die - forcing termination");
		kill(vrrp_child, SIGKILL);
	}
#endif
#ifdef _WITH_LVS_
	if (checkers_child) {
		log_message(LOG_INFO, "checker process failed to die - forcing termination");
		kill(checkers_child, SIGKILL);
	}
#endif
#ifdef _WITH_BFD_
	if (bfd_child) {
		log_message(LOG_INFO, "bfd process failed to die - forcing termination");
		kill(bfd_child, SIGKILL);
	}
#endif

#ifndef HAVE_SIGNALFD
	if (!sigismember(&old_set, SIGCHLD))
		sigmask_func(SIG_UNBLOCK, &child_wait, NULL);
#endif
}
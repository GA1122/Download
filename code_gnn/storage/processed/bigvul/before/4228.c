__svc_clean_idle(fd_set *fds, int timeout, bool_t cleanblock)
{
	int i, ncleaned;
	SVCXPRT *xprt, *least_active;
	struct timeval tv, tdiff, tmax;
	struct cf_conn *cd;

	gettimeofday(&tv, NULL);
	tmax.tv_sec = tmax.tv_usec = 0;
	least_active = NULL;
	rwlock_wrlock(&svc_fd_lock);
	for (i = ncleaned = 0; i <= svc_maxfd; i++) {
		if (FD_ISSET(i, fds)) {
			xprt = __svc_xports[i];
			if (xprt == NULL || xprt->xp_ops == NULL ||
			    xprt->xp_ops->xp_recv != svc_vc_recv)
				continue;
			cd = (struct cf_conn *)xprt->xp_p1;
			if (!cleanblock && !cd->nonblock)
				continue;
			if (timeout == 0) {
				timersub(&tv, &cd->last_recv_time, &tdiff);
				if (timercmp(&tdiff, &tmax, >)) {
					tmax = tdiff;
					least_active = xprt;
				}
				continue;
			}
			if (tv.tv_sec - cd->last_recv_time.tv_sec > timeout) {
				__xprt_unregister_unlocked(xprt);
				__svc_vc_dodestroy(xprt);
				ncleaned++;
			}
		}
	}
	if (timeout == 0 && least_active != NULL) {
		__xprt_unregister_unlocked(least_active);
		__svc_vc_dodestroy(least_active);
		ncleaned++;
	}
	rwlock_unlock(&svc_fd_lock);
	return ncleaned > 0 ? TRUE : FALSE;
}

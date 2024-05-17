static int do_chown_cgroup(const char *controller, const char *cgroup_path,
		uid_t newuid)
{
	int sv[2] = {-1, -1}, optval = 1, ret = -1;
	char buf[1];
	struct pollfd fds;

	if (socketpair(AF_UNIX, SOCK_DGRAM, 0, sv) < 0) {
		SYSERROR("Error creating socketpair");
		goto out;
	}
	if (setsockopt(sv[1], SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval)) == -1) {
		SYSERROR("setsockopt failed");
		goto out;
	}
	if (setsockopt(sv[0], SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval)) == -1) {
		SYSERROR("setsockopt failed");
		goto out;
	}
	if ( cgmanager_chown_scm_sync(NULL, cgroup_manager, controller,
				       cgroup_path, sv[1]) != 0) {
		NihError *nerr;
		nerr = nih_error_get();
		ERROR("call to cgmanager_chown_scm_sync failed: %s", nerr->message);
		nih_free(nerr);
		goto out;
	}
	 

	fds.fd = sv[0];
	fds.events = POLLIN;
	fds.revents = 0;
	if (poll(&fds, 1, -1) <= 0) {
		ERROR("Error getting go-ahead from server: %s", strerror(errno));
		goto out;
	}
	if (read(sv[0], &buf, 1) != 1) {
		ERROR("Error getting reply from server over socketpair");
		goto out;
	}
	if (send_creds(sv[0], getpid(), getuid(), getgid())) {
		SYSERROR("%s: Error sending pid over SCM_CREDENTIAL", __func__);
		goto out;
	}
	fds.fd = sv[0];
	fds.events = POLLIN;
	fds.revents = 0;
	if (poll(&fds, 1, -1) <= 0) {
		ERROR("Error getting go-ahead from server: %s", strerror(errno));
		goto out;
	}
	if (read(sv[0], &buf, 1) != 1) {
		ERROR("Error getting reply from server over socketpair");
		goto out;
	}
	if (send_creds(sv[0], getpid(), newuid, 0)) {
		SYSERROR("%s: Error sending pid over SCM_CREDENTIAL", __func__);
		goto out;
	}
	fds.fd = sv[0];
	fds.events = POLLIN;
	fds.revents = 0;
	if (poll(&fds, 1, -1) <= 0) {
		ERROR("Error getting go-ahead from server: %s", strerror(errno));
		goto out;
	}
	ret = read(sv[0], buf, 1);
out:
	close(sv[0]);
	close(sv[1]);
	if (ret == 1 && *buf == '1')
		return 0;
	return -1;
}
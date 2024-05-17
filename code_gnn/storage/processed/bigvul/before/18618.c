SYSCALL_DEFINE3(recvmsg, int, fd, struct msghdr __user *, msg,
		unsigned int, flags)
{
	int fput_needed, err;
	struct msghdr msg_sys;
	struct socket *sock = sockfd_lookup_light(fd, &err, &fput_needed);

	if (!sock)
		goto out;

	err = __sys_recvmsg(sock, msg, &msg_sys, flags, 0);

	fput_light(sock->file, fput_needed);
out:
	return err;
}
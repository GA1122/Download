asmlinkage long sys_oabi_sendmsg(int fd, struct msghdr __user *msg, unsigned flags)
{
	struct sockaddr __user *addr;
	int msg_namelen;
	sa_family_t sa_family;
	if (msg &&
	    get_user(msg_namelen, &msg->msg_namelen) == 0 &&
	    msg_namelen == 112 &&
	    get_user(addr, &msg->msg_name) == 0 &&
	    get_user(sa_family, &addr->sa_family) == 0 &&
	    sa_family == AF_UNIX)
	{
		 
		put_user(110, &msg->msg_namelen);
	}
	return sys_sendmsg(fd, msg, flags);
}

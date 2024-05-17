static int atalk_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	 
	if (cmd == SIOCATALKDIFADDR)
		return atalk_ioctl(sock, cmd, (unsigned long)compat_ptr(arg));

	return -ENOIOCTLCMD;
}

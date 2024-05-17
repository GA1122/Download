static int irda_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	 
	return -ENOIOCTLCMD;
}

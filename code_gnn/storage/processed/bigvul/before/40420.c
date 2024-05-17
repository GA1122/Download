static int ipx_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	 
	switch (cmd) {
	case SIOCAIPXITFCRT:
	case SIOCAIPXPRISLT:
	case SIOCIPXCFGDATA:
	case SIOCIPXNCPCONN:
		return ipx_ioctl(sock, cmd, arg);
	default:
		return -ENOIOCTLCMD;
	}
}

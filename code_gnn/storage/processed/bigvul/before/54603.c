struct sock *unix_get_socket(struct file *filp)
{
	struct sock *u_sock = NULL;
	struct inode *inode = file_inode(filp);

	 
	if (S_ISSOCK(inode->i_mode) && !(filp->f_mode & FMODE_PATH)) {
		struct socket *sock = SOCKET_I(inode);
		struct sock *s = sock->sk;

		 
		if (s && sock->ops && sock->ops->family == PF_UNIX)
			u_sock = s;
	}
	return u_sock;
}
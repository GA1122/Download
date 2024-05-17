struct socket *sock_from_file(struct file *file, int *err)
{
	if (file->f_op == &socket_file_ops)
		return file->private_data;	 

	*err = -ENOTSOCK;
	return NULL;
}

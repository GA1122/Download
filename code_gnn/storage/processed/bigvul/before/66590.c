static int get_chars(u32 vtermno, char *buf, int count)
{
	struct port *port;

	 
	if (unlikely(early_put_chars))
		return 0;

	port = find_port_by_vtermno(vtermno);
	if (!port)
		return -EPIPE;

	 
	BUG_ON(!port->in_vq);

	return fill_readbuf(port, (__force char __user *)buf, count, false);
}

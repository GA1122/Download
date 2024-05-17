static ssize_t port_fops_read(struct file *filp, char __user *ubuf,
			      size_t count, loff_t *offp)
{
	struct port *port;
	ssize_t ret;

	port = filp->private_data;

	 
	if (!port->guest_connected)
		return -ENODEV;

	if (!port_has_data(port)) {
		 
		if (!port->host_connected)
			return 0;
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		ret = wait_event_freezable(port->waitqueue,
					   !will_read_block(port));
		if (ret < 0)
			return ret;
	}
	 
	if (!port->guest_connected)
		return -ENODEV;
	 
	if (!port_has_data(port) && !port->host_connected)
		return 0;

	return fill_readbuf(port, ubuf, count, true);
}

static int port_fops_open(struct inode *inode, struct file *filp)
{
	struct cdev *cdev = inode->i_cdev;
	struct port *port;
	int ret;

	 
	port = find_port_by_devt(cdev->dev);
	if (!port) {
		 
		return -ENXIO;
	}
	filp->private_data = port;

	 
	if (is_console_port(port)) {
		ret = -ENXIO;
		goto out;
	}

	 
	spin_lock_irq(&port->inbuf_lock);
	if (port->guest_connected) {
		spin_unlock_irq(&port->inbuf_lock);
		ret = -EBUSY;
		goto out;
	}

	port->guest_connected = true;
	spin_unlock_irq(&port->inbuf_lock);

	spin_lock_irq(&port->outvq_lock);
	 
	reclaim_consumed_buffers(port);
	spin_unlock_irq(&port->outvq_lock);

	nonseekable_open(inode, filp);

	 
	send_control_msg(filp->private_data, VIRTIO_CONSOLE_PORT_OPEN, 1);

	return 0;
out:
	kref_put(&port->kref, remove_port);
	return ret;
}

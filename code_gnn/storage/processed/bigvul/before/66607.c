static int port_fops_release(struct inode *inode, struct file *filp)
{
	struct port *port;

	port = filp->private_data;

	 
	send_control_msg(port, VIRTIO_CONSOLE_PORT_OPEN, 0);

	spin_lock_irq(&port->inbuf_lock);
	port->guest_connected = false;

	discard_port_data(port);

	spin_unlock_irq(&port->inbuf_lock);

	spin_lock_irq(&port->outvq_lock);
	reclaim_consumed_buffers(port);
	spin_unlock_irq(&port->outvq_lock);

	reclaim_dma_bufs();
	 
	kref_put(&port->kref, remove_port);

	return 0;
}

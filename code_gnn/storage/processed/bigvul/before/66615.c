static void remove_port_data(struct port *port)
{
	struct port_buffer *buf;

	spin_lock_irq(&port->inbuf_lock);
	 
	discard_port_data(port);
	spin_unlock_irq(&port->inbuf_lock);

	 
	do {
		spin_lock_irq(&port->inbuf_lock);
		buf = virtqueue_detach_unused_buf(port->in_vq);
		spin_unlock_irq(&port->inbuf_lock);
		if (buf)
			free_buf(buf, true);
	} while (buf);

	spin_lock_irq(&port->outvq_lock);
	reclaim_consumed_buffers(port);
	spin_unlock_irq(&port->outvq_lock);

	 
	do {
		spin_lock_irq(&port->outvq_lock);
		buf = virtqueue_detach_unused_buf(port->out_vq);
		spin_unlock_irq(&port->outvq_lock);
		if (buf)
			free_buf(buf, true);
	} while (buf);
}

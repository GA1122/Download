static void reclaim_consumed_buffers(struct port *port)
{
	struct port_buffer *buf;
	unsigned int len;

	if (!port->portdev) {
		 
		return;
	}
	while ((buf = virtqueue_get_buf(port->out_vq, &len))) {
		free_buf(buf, false);
		port->outvq_full = false;
	}
}
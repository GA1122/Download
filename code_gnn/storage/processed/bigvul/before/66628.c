static bool will_write_block(struct port *port)
{
	bool ret;

	if (!port->guest_connected) {
		 
		return false;
	}
	if (!port->host_connected)
		return true;

	spin_lock_irq(&port->outvq_lock);
	 
	reclaim_consumed_buffers(port);
	ret = port->outvq_full;
	spin_unlock_irq(&port->outvq_lock);

	return ret;
}

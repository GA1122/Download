static ssize_t send_control_msg(struct port *port, unsigned int event,
				unsigned int value)
{
	 
	if (port->portdev)
		return __send_control_msg(port->portdev, port->id, event, value);
	return 0;
}

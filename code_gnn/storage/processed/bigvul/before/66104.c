static int ti_do_config(struct edgeport_port *port, int feature, int on)
{
	int port_number = port->port->port_number;

	on = !!on;	 
	return send_cmd(port->port->serial->dev,
			feature, (__u8)(UMPM_UART1_PORT + port_number),
			on, NULL, 0);
}

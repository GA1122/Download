static int analyze_baud_rate(struct usb_serial_port *port, speed_t new_rate)
{
	struct cypress_private *priv;
	priv = usb_get_serial_port_data(port);

	if (unstable_bauds)
		return new_rate;

	 
	if (is_frwd(port->serial->dev))
		return new_rate;

	 
	if (port->serial->dev->speed == USB_SPEED_LOW) {
		 
		if (new_rate > 4800) {
			dev_dbg(&port->dev,
				"%s - failed setting baud rate, device incapable speed %d\n",
				__func__, new_rate);
			return -1;
		}
	}
	switch (priv->chiptype) {
	case CT_EARTHMATE:
		if (new_rate <= 600) {
			 
			dev_dbg(&port->dev,
				"%s - failed setting baud rate, unsupported speed of %d on Earthmate GPS\n",
				__func__, new_rate);
			return -1;
		}
		break;
	default:
		break;
	}
	return new_rate;
}

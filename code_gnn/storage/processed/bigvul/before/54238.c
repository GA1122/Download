static int cypress_earthmate_port_probe(struct usb_serial_port *port)
{
	struct usb_serial *serial = port->serial;
	struct cypress_private *priv;
	int ret;

	ret = cypress_generic_port_probe(port);
	if (ret) {
		dev_dbg(&port->dev, "%s - Failed setting up port\n", __func__);
		return ret;
	}

	priv = usb_get_serial_port_data(port);
	priv->chiptype = CT_EARTHMATE;
	 
	priv->pkt_fmt = packet_format_1;
	if (serial->dev->descriptor.idProduct !=
				cpu_to_le16(PRODUCT_ID_EARTHMATEUSB)) {
		 
		dev_dbg(&port->dev,
			"%s - Marking this device as unsafe for GET_CONFIG commands\n",
			__func__);
		priv->get_cfg_unsafe = !0;
	}

	return 0;
}
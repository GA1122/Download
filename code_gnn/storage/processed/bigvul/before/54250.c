static int cypress_write(struct tty_struct *tty, struct usb_serial_port *port,
					const unsigned char *buf, int count)
{
	struct cypress_private *priv = usb_get_serial_port_data(port);

	dev_dbg(&port->dev, "%s - %d bytes\n", __func__, count);

	 
	if (priv->cmd_ctrl) {
		count = 0;
		goto finish;
	}

	if (!count)
		return count;

	count = kfifo_in_locked(&priv->write_fifo, buf, count, &priv->lock);

finish:
	cypress_send(port);

	return count;
}  

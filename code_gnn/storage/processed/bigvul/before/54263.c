static void mct_u232_read_int_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct mct_u232_private *priv = usb_get_serial_port_data(port);
	unsigned char *data = urb->transfer_buffer;
	int retval;
	int status = urb->status;
	unsigned long flags;

	switch (status) {
	case 0:
		 
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		 
		dev_dbg(&port->dev, "%s - urb shutting down with status: %d\n",
			__func__, status);
		return;
	default:
		dev_dbg(&port->dev, "%s - nonzero urb status received: %d\n",
			__func__, status);
		goto exit;
	}

	usb_serial_debug_data(&port->dev, __func__, urb->actual_length, data);

	 
	if (urb->transfer_buffer_length > 2) {
		if (urb->actual_length) {
			tty_insert_flip_string(&port->port, data,
					urb->actual_length);
			tty_flip_buffer_push(&port->port);
		}
		goto exit;
	}

	 
	spin_lock_irqsave(&priv->lock, flags);
	priv->last_msr = data[MCT_U232_MSR_INDEX];

	 
	mct_u232_msr_to_state(port, &priv->control_state, priv->last_msr);

	mct_u232_msr_to_icount(&port->icount, priv->last_msr);

#if 0
	 
	 
	priv->last_lsr = data[MCT_U232_LSR_INDEX];
	 
	if (priv->last_lsr & MCT_U232_LSR_ERR) {
		tty = tty_port_tty_get(&port->port);
		 
		if (priv->last_lsr & MCT_U232_LSR_OE) {
		}
		 
		if (priv->last_lsr & MCT_U232_LSR_PE) {
		}
		 
		if (priv->last_lsr & MCT_U232_LSR_FE) {
		}
		 
		if (priv->last_lsr & MCT_U232_LSR_BI) {
		}
		tty_kref_put(tty);
	}
#endif
	wake_up_interruptible(&port->port.delta_msr_wait);
	spin_unlock_irqrestore(&priv->lock, flags);
exit:
	retval = usb_submit_urb(urb, GFP_ATOMIC);
	if (retval)
		dev_err(&port->dev,
			"%s - usb_submit_urb failed with result %d\n",
			__func__, retval);
}  

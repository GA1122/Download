static void dw2102_disconnect(struct usb_interface *intf)
{
	struct dvb_usb_device *d = usb_get_intfdata(intf);
	struct dw2102_state *st = (struct dw2102_state *)d->priv;
	struct i2c_client *client;

	 
	client = st->i2c_client_tuner;
	if (client) {
		module_put(client->dev.driver->owner);
		i2c_unregister_device(client);
	}

	 
	client = st->i2c_client_demod;
	if (client) {
		module_put(client->dev.driver->owner);
		i2c_unregister_device(client);
	}

	dvb_usb_device_exit(intf);
}
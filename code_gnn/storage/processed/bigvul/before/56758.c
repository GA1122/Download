static void hub_port_logical_disconnect(struct usb_hub *hub, int port1)
{
	dev_dbg(&hub->ports[port1 - 1]->dev, "logical disconnect\n");
	hub_port_disable(hub, port1, 1);

	 

	set_bit(port1, hub->change_bits);
	kick_hub_wq(hub);
}

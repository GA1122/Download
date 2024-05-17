static void ipmi_poll(struct ipmi_smi *intf)
{
	if (intf->handlers->poll)
		intf->handlers->poll(intf->send_info);
	 
	handle_new_recv_msgs(intf);
}

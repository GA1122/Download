static void ipmi_request_event(struct ipmi_smi *intf)
{
	 
	if (intf->maintenance_mode_enable)
		return;

	if (!intf->in_shutdown)
		intf->handlers->request_events(intf->send_info);
}
static void smi_send(struct ipmi_smi *intf,
		     const struct ipmi_smi_handlers *handlers,
		     struct ipmi_smi_msg *smi_msg, int priority)
{
	int run_to_completion = intf->run_to_completion;

	if (run_to_completion) {
		smi_msg = smi_add_send_msg(intf, smi_msg, priority);
	} else {
		unsigned long flags;

		spin_lock_irqsave(&intf->xmit_msgs_lock, flags);
		smi_msg = smi_add_send_msg(intf, smi_msg, priority);
		spin_unlock_irqrestore(&intf->xmit_msgs_lock, flags);
	}

	if (smi_msg)
		handlers->sender(intf->send_info, smi_msg);
}
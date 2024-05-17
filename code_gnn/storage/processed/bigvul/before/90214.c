static void deliver_recv_msg(struct smi_info *smi_info,
			     struct ipmi_smi_msg *msg)
{
	 
	ipmi_smi_msg_received(smi_info->intf, msg);
}

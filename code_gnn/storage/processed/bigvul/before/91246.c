static int handle_lan_get_msg_rsp(struct ipmi_smi *intf,
				  struct ipmi_smi_msg *msg)
{
	struct ipmi_lan_addr  lan_addr;
	struct ipmi_recv_msg  *recv_msg;


	 
	if (msg->rsp_size < 13) {
		 
		ipmi_inc_stat(intf, invalid_lan_responses);
		return 0;
	}

	if (msg->rsp[2] != 0) {
		 
		return 0;
	}

	lan_addr.addr_type = IPMI_LAN_ADDR_TYPE;
	lan_addr.session_handle = msg->rsp[4];
	lan_addr.remote_SWID = msg->rsp[8];
	lan_addr.local_SWID = msg->rsp[5];
	lan_addr.channel = msg->rsp[3] & 0x0f;
	lan_addr.privilege = msg->rsp[3] >> 4;
	lan_addr.lun = msg->rsp[9] & 3;

	 
	if (intf_find_seq(intf,
			  msg->rsp[9] >> 2,
			  msg->rsp[3] & 0x0f,
			  msg->rsp[10],
			  (msg->rsp[6] >> 2) & (~1),
			  (struct ipmi_addr *) &lan_addr,
			  &recv_msg)) {
		 
		ipmi_inc_stat(intf, unhandled_lan_responses);
		return 0;
	}

	memcpy(recv_msg->msg_data, &msg->rsp[11], msg->rsp_size - 11);
	 
	recv_msg->msg.netfn = msg->rsp[6] >> 2;
	recv_msg->msg.data = recv_msg->msg_data;
	recv_msg->msg.data_len = msg->rsp_size - 12;
	recv_msg->recv_type = IPMI_RESPONSE_RECV_TYPE;
	if (deliver_response(intf, recv_msg))
		ipmi_inc_stat(intf, unhandled_lan_responses);
	else
		ipmi_inc_stat(intf, handled_lan_responses);

	return 0;
}

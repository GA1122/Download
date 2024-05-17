static int handle_oem_get_msg_cmd(struct ipmi_smi *intf,
				  struct ipmi_smi_msg *msg)
{
	struct cmd_rcvr       *rcvr;
	int                   rv = 0;
	unsigned char         netfn;
	unsigned char         cmd;
	unsigned char         chan;
	struct ipmi_user *user = NULL;
	struct ipmi_system_interface_addr *smi_addr;
	struct ipmi_recv_msg  *recv_msg;

	 
	if (msg->rsp_size < 4) {
		 
		ipmi_inc_stat(intf, invalid_commands);
		return 0;
	}

	if (msg->rsp[2] != 0) {
		 
		return 0;
	}

	 
	netfn = msg->rsp[0] >> 2;
	cmd = msg->rsp[1];
	chan = msg->rsp[3] & 0xf;

	rcu_read_lock();
	rcvr = find_cmd_rcvr(intf, netfn, cmd, chan);
	if (rcvr) {
		user = rcvr->user;
		kref_get(&user->refcount);
	} else
		user = NULL;
	rcu_read_unlock();

	if (user == NULL) {
		 
		ipmi_inc_stat(intf, unhandled_commands);

		 

		rv = 0;
	} else {
		recv_msg = ipmi_alloc_recv_msg();
		if (!recv_msg) {
			 
			rv = 1;
			kref_put(&user->refcount, free_user);
		} else {
			 
			smi_addr = ((struct ipmi_system_interface_addr *)
				    &recv_msg->addr);
			smi_addr->addr_type = IPMI_SYSTEM_INTERFACE_ADDR_TYPE;
			smi_addr->channel = IPMI_BMC_CHANNEL;
			smi_addr->lun = msg->rsp[0] & 3;

			recv_msg->user = user;
			recv_msg->user_msg_data = NULL;
			recv_msg->recv_type = IPMI_OEM_RECV_TYPE;
			recv_msg->msg.netfn = msg->rsp[0] >> 2;
			recv_msg->msg.cmd = msg->rsp[1];
			recv_msg->msg.data = recv_msg->msg_data;

			 
			recv_msg->msg.data_len = msg->rsp_size - 4;
			memcpy(recv_msg->msg_data, &msg->rsp[4],
			       msg->rsp_size - 4);
			if (deliver_response(intf, recv_msg))
				ipmi_inc_stat(intf, unhandled_commands);
			else
				ipmi_inc_stat(intf, handled_commands);
		}
	}

	return rv;
}

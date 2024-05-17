static int handle_ipmb_get_msg_cmd(struct ipmi_smi *intf,
				   struct ipmi_smi_msg *msg)
{
	struct cmd_rcvr          *rcvr;
	int                      rv = 0;
	unsigned char            netfn;
	unsigned char            cmd;
	unsigned char            chan;
	struct ipmi_user         *user = NULL;
	struct ipmi_ipmb_addr    *ipmb_addr;
	struct ipmi_recv_msg     *recv_msg;

	if (msg->rsp_size < 10) {
		 
		ipmi_inc_stat(intf, invalid_commands);
		return 0;
	}

	if (msg->rsp[2] != 0) {
		 
		return 0;
	}

	netfn = msg->rsp[4] >> 2;
	cmd = msg->rsp[8];
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

		msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
		msg->data[1] = IPMI_SEND_MSG_CMD;
		msg->data[2] = msg->rsp[3];
		msg->data[3] = msg->rsp[6];
		msg->data[4] = ((netfn + 1) << 2) | (msg->rsp[7] & 0x3);
		msg->data[5] = ipmb_checksum(&msg->data[3], 2);
		msg->data[6] = intf->addrinfo[msg->rsp[3] & 0xf].address;
		 
		msg->data[7] = (msg->rsp[7] & 0xfc) | (msg->rsp[4] & 0x3);
		msg->data[8] = msg->rsp[8];  
		msg->data[9] = IPMI_INVALID_CMD_COMPLETION_CODE;
		msg->data[10] = ipmb_checksum(&msg->data[6], 4);
		msg->data_size = 11;

		ipmi_debug_msg("Invalid command:", msg->data, msg->data_size);

		rcu_read_lock();
		if (!intf->in_shutdown) {
			smi_send(intf, intf->handlers, msg, 0);
			 
			rv = -1;
		}
		rcu_read_unlock();
	} else {
		recv_msg = ipmi_alloc_recv_msg();
		if (!recv_msg) {
			 
			rv = 1;
			kref_put(&user->refcount, free_user);
		} else {
			 
			ipmb_addr = (struct ipmi_ipmb_addr *) &recv_msg->addr;
			ipmb_addr->addr_type = IPMI_IPMB_ADDR_TYPE;
			ipmb_addr->slave_addr = msg->rsp[6];
			ipmb_addr->lun = msg->rsp[7] & 3;
			ipmb_addr->channel = msg->rsp[3] & 0xf;

			 
			recv_msg->user = user;
			recv_msg->recv_type = IPMI_CMD_RECV_TYPE;
			recv_msg->msgid = msg->rsp[7] >> 2;
			recv_msg->msg.netfn = msg->rsp[4] >> 2;
			recv_msg->msg.cmd = msg->rsp[8];
			recv_msg->msg.data = recv_msg->msg_data;

			 
			recv_msg->msg.data_len = msg->rsp_size - 10;
			memcpy(recv_msg->msg_data, &msg->rsp[9],
			       msg->rsp_size - 10);
			if (deliver_response(intf, recv_msg))
				ipmi_inc_stat(intf, unhandled_commands);
			else
				ipmi_inc_stat(intf, handled_commands);
		}
	}

	return rv;
}
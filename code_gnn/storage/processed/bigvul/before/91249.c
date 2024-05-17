static int handle_one_recv_msg(struct ipmi_smi *intf,
			       struct ipmi_smi_msg *msg)
{
	int requeue;
	int chan;

	ipmi_debug_msg("Recv:", msg->rsp, msg->rsp_size);
	if (msg->rsp_size < 2) {
		 
		dev_warn(intf->si_dev,
			 "BMC returned too small a message for netfn %x cmd %x, got %d bytes\n",
			 (msg->data[0] >> 2) | 1, msg->data[1], msg->rsp_size);

		 
		msg->rsp[0] = msg->data[0] | (1 << 2);
		msg->rsp[1] = msg->data[1];
		msg->rsp[2] = IPMI_ERR_UNSPECIFIED;
		msg->rsp_size = 3;
	} else if (((msg->rsp[0] >> 2) != ((msg->data[0] >> 2) | 1))
		   || (msg->rsp[1] != msg->data[1])) {
		 
		dev_warn(intf->si_dev,
			 "BMC returned incorrect response, expected netfn %x cmd %x, got netfn %x cmd %x\n",
			 (msg->data[0] >> 2) | 1, msg->data[1],
			 msg->rsp[0] >> 2, msg->rsp[1]);

		 
		msg->rsp[0] = msg->data[0] | (1 << 2);
		msg->rsp[1] = msg->data[1];
		msg->rsp[2] = IPMI_ERR_UNSPECIFIED;
		msg->rsp_size = 3;
	}

	if ((msg->rsp[0] == ((IPMI_NETFN_APP_REQUEST|1) << 2))
	    && (msg->rsp[1] == IPMI_SEND_MSG_CMD)
	    && (msg->user_data != NULL)) {
		 
		struct ipmi_recv_msg *recv_msg = msg->user_data;

		requeue = 0;
		if (msg->rsp_size < 2)
			 
			goto out;

		chan = msg->data[2] & 0x0f;
		if (chan >= IPMI_MAX_CHANNELS)
			 
			goto out;

		if (!recv_msg)
			goto out;

		recv_msg->recv_type = IPMI_RESPONSE_RESPONSE_TYPE;
		recv_msg->msg.data = recv_msg->msg_data;
		recv_msg->msg.data_len = 1;
		recv_msg->msg_data[0] = msg->rsp[2];
		deliver_local_response(intf, recv_msg);
	} else if ((msg->rsp[0] == ((IPMI_NETFN_APP_REQUEST|1) << 2))
		   && (msg->rsp[1] == IPMI_GET_MSG_CMD)) {
		struct ipmi_channel   *chans;

		 
		chan = msg->rsp[3] & 0xf;
		if (chan >= IPMI_MAX_CHANNELS) {
			 
			requeue = 0;
			goto out;
		}

		 
		if (!intf->channels_ready) {
			requeue = 0;  
			goto out;
		}

		chans = READ_ONCE(intf->channel_list)->c;

		switch (chans[chan].medium) {
		case IPMI_CHANNEL_MEDIUM_IPMB:
			if (msg->rsp[4] & 0x04) {
				 
				requeue = handle_ipmb_get_msg_rsp(intf, msg);
			} else {
				 
				requeue = handle_ipmb_get_msg_cmd(intf, msg);
			}
			break;

		case IPMI_CHANNEL_MEDIUM_8023LAN:
		case IPMI_CHANNEL_MEDIUM_ASYNC:
			if (msg->rsp[6] & 0x04) {
				 
				requeue = handle_lan_get_msg_rsp(intf, msg);
			} else {
				 
				requeue = handle_lan_get_msg_cmd(intf, msg);
			}
			break;

		default:
			 
			if ((chans[chan].medium >= IPMI_CHANNEL_MEDIUM_OEM_MIN)
			    && (chans[chan].medium
				<= IPMI_CHANNEL_MEDIUM_OEM_MAX)) {
				requeue = handle_oem_get_msg_cmd(intf, msg);
			} else {
				 
				requeue = 0;
			}
		}

	} else if ((msg->rsp[0] == ((IPMI_NETFN_APP_REQUEST|1) << 2))
		   && (msg->rsp[1] == IPMI_READ_EVENT_MSG_BUFFER_CMD)) {
		 
		requeue = handle_read_event_rsp(intf, msg);
	} else {
		 
		requeue = handle_bmc_rsp(intf, msg);
	}

 out:
	return requeue;
}

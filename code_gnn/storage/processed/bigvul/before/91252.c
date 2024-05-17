static int i_ipmi_req_lan(struct ipmi_smi        *intf,
			  struct ipmi_addr       *addr,
			  long                   msgid,
			  struct kernel_ipmi_msg *msg,
			  struct ipmi_smi_msg    *smi_msg,
			  struct ipmi_recv_msg   *recv_msg,
			  unsigned char          source_lun,
			  int                    retries,
			  unsigned int           retry_time_ms)
{
	struct ipmi_lan_addr  *lan_addr;
	unsigned char ipmb_seq;
	long seqid;
	struct ipmi_channel *chans;
	int rv = 0;

	if (addr->channel >= IPMI_MAX_CHANNELS) {
		ipmi_inc_stat(intf, sent_invalid_commands);
		return -EINVAL;
	}

	chans = READ_ONCE(intf->channel_list)->c;

	if ((chans[addr->channel].medium
				!= IPMI_CHANNEL_MEDIUM_8023LAN)
			&& (chans[addr->channel].medium
			    != IPMI_CHANNEL_MEDIUM_ASYNC)) {
		ipmi_inc_stat(intf, sent_invalid_commands);
		return -EINVAL;
	}

	 
	if ((msg->data_len + 12) > IPMI_MAX_MSG_LENGTH) {
		ipmi_inc_stat(intf, sent_invalid_commands);
		return -EMSGSIZE;
	}

	lan_addr = (struct ipmi_lan_addr *) addr;
	if (lan_addr->lun > 3) {
		ipmi_inc_stat(intf, sent_invalid_commands);
		return -EINVAL;
	}

	memcpy(&recv_msg->addr, lan_addr, sizeof(*lan_addr));

	if (recv_msg->msg.netfn & 0x1) {
		 
		ipmi_inc_stat(intf, sent_lan_responses);
		format_lan_msg(smi_msg, msg, lan_addr, msgid,
			       msgid, source_lun);

		 
		smi_msg->user_data = recv_msg;
	} else {
		 
		unsigned long flags;

		spin_lock_irqsave(&intf->seq_lock, flags);

		 
		rv = intf_next_seq(intf,
				   recv_msg,
				   retry_time_ms,
				   retries,
				   0,
				   &ipmb_seq,
				   &seqid);
		if (rv)
			 
			goto out_err;

		ipmi_inc_stat(intf, sent_lan_commands);

		 
		format_lan_msg(smi_msg, msg, lan_addr,
			       STORE_SEQ_IN_MSGID(ipmb_seq, seqid),
			       ipmb_seq, source_lun);

		 
		memcpy(recv_msg->msg_data, smi_msg->data,
		       smi_msg->data_size);
		recv_msg->msg.data = recv_msg->msg_data;
		recv_msg->msg.data_len = smi_msg->data_size;

		 
out_err:
		spin_unlock_irqrestore(&intf->seq_lock, flags);
	}

	return rv;
}

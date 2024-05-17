void ipmi_smi_msg_received(struct ipmi_smi *intf,
			   struct ipmi_smi_msg *msg)
{
	unsigned long flags = 0;  
	int run_to_completion = intf->run_to_completion;

	if ((msg->data_size >= 2)
	    && (msg->data[0] == (IPMI_NETFN_APP_REQUEST << 2))
	    && (msg->data[1] == IPMI_SEND_MSG_CMD)
	    && (msg->user_data == NULL)) {

		if (intf->in_shutdown)
			goto free_msg;

		 

		 
		if ((msg->rsp_size >= 3) && (msg->rsp[2] != 0)
		    && (msg->rsp[2] != IPMI_NODE_BUSY_ERR)
		    && (msg->rsp[2] != IPMI_LOST_ARBITRATION_ERR)
		    && (msg->rsp[2] != IPMI_BUS_ERR)
		    && (msg->rsp[2] != IPMI_NAK_ON_WRITE_ERR)) {
			int ch = msg->rsp[3] & 0xf;
			struct ipmi_channel *chans;

			 

			chans = READ_ONCE(intf->channel_list)->c;
			if ((chans[ch].medium == IPMI_CHANNEL_MEDIUM_8023LAN)
			    || (chans[ch].medium == IPMI_CHANNEL_MEDIUM_ASYNC))
				ipmi_inc_stat(intf, sent_lan_command_errs);
			else
				ipmi_inc_stat(intf, sent_ipmb_command_errs);
			intf_err_seq(intf, msg->msgid, msg->rsp[2]);
		} else
			 
			intf_start_seq_timer(intf, msg->msgid);

free_msg:
		ipmi_free_smi_msg(msg);
	} else {
		 
		if (!run_to_completion)
			spin_lock_irqsave(&intf->waiting_rcv_msgs_lock, flags);
		list_add_tail(&msg->link, &intf->waiting_rcv_msgs);
		if (!run_to_completion)
			spin_unlock_irqrestore(&intf->waiting_rcv_msgs_lock,
					       flags);
	}

	if (!run_to_completion)
		spin_lock_irqsave(&intf->xmit_msgs_lock, flags);
	 
	if (msg == intf->curr_msg)
		intf->curr_msg = NULL;
	if (!run_to_completion)
		spin_unlock_irqrestore(&intf->xmit_msgs_lock, flags);

	if (run_to_completion)
		smi_recv_tasklet((unsigned long) intf);
	else
		tasklet_schedule(&intf->recv_tasklet);
}

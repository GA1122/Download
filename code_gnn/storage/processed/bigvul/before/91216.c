static void check_msg_timeout(struct ipmi_smi *intf, struct seq_table *ent,
			      struct list_head *timeouts,
			      unsigned long timeout_period,
			      int slot, unsigned long *flags,
			      unsigned int *waiting_msgs)
{
	struct ipmi_recv_msg *msg;

	if (intf->in_shutdown)
		return;

	if (!ent->inuse)
		return;

	if (timeout_period < ent->timeout) {
		ent->timeout -= timeout_period;
		(*waiting_msgs)++;
		return;
	}

	if (ent->retries_left == 0) {
		 
		ent->inuse = 0;
		msg = ent->recv_msg;
		list_add_tail(&msg->link, timeouts);
		if (ent->broadcast)
			ipmi_inc_stat(intf, timed_out_ipmb_broadcasts);
		else if (is_lan_addr(&ent->recv_msg->addr))
			ipmi_inc_stat(intf, timed_out_lan_commands);
		else
			ipmi_inc_stat(intf, timed_out_ipmb_commands);
	} else {
		struct ipmi_smi_msg *smi_msg;
		 

		(*waiting_msgs)++;

		 
		ent->timeout = MAX_MSG_TIMEOUT;
		ent->retries_left--;
		smi_msg = smi_from_recv_msg(intf, ent->recv_msg, slot,
					    ent->seqid);
		if (!smi_msg) {
			if (is_lan_addr(&ent->recv_msg->addr))
				ipmi_inc_stat(intf,
					      dropped_rexmit_lan_commands);
			else
				ipmi_inc_stat(intf,
					      dropped_rexmit_ipmb_commands);
			return;
		}

		spin_unlock_irqrestore(&intf->seq_lock, *flags);

		 
		if (intf->handlers) {
			if (is_lan_addr(&ent->recv_msg->addr))
				ipmi_inc_stat(intf,
					      retransmitted_lan_commands);
			else
				ipmi_inc_stat(intf,
					      retransmitted_ipmb_commands);

			smi_send(intf, intf->handlers, smi_msg, 0);
		} else
			ipmi_free_smi_msg(smi_msg);

		spin_lock_irqsave(&intf->seq_lock, *flags);
	}
}
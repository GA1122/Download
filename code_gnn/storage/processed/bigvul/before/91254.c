static int i_ipmi_request(struct ipmi_user     *user,
			  struct ipmi_smi      *intf,
			  struct ipmi_addr     *addr,
			  long                 msgid,
			  struct kernel_ipmi_msg *msg,
			  void                 *user_msg_data,
			  void                 *supplied_smi,
			  struct ipmi_recv_msg *supplied_recv,
			  int                  priority,
			  unsigned char        source_address,
			  unsigned char        source_lun,
			  int                  retries,
			  unsigned int         retry_time_ms)
{
	struct ipmi_smi_msg *smi_msg;
	struct ipmi_recv_msg *recv_msg;
	int rv = 0;

	if (supplied_recv)
		recv_msg = supplied_recv;
	else {
		recv_msg = ipmi_alloc_recv_msg();
		if (recv_msg == NULL) {
			rv = -ENOMEM;
			goto out;
		}
	}
	recv_msg->user_msg_data = user_msg_data;

	if (supplied_smi)
		smi_msg = (struct ipmi_smi_msg *) supplied_smi;
	else {
		smi_msg = ipmi_alloc_smi_msg();
		if (smi_msg == NULL) {
			ipmi_free_recv_msg(recv_msg);
			rv = -ENOMEM;
			goto out;
		}
	}

	rcu_read_lock();
	if (intf->in_shutdown) {
		rv = -ENODEV;
		goto out_err;
	}

	recv_msg->user = user;
	if (user)
		 
		kref_get(&user->refcount);
	recv_msg->msgid = msgid;
	 
	recv_msg->msg = *msg;

	if (addr->addr_type == IPMI_SYSTEM_INTERFACE_ADDR_TYPE) {
		rv = i_ipmi_req_sysintf(intf, addr, msgid, msg, smi_msg,
					recv_msg, retries, retry_time_ms);
	} else if (is_ipmb_addr(addr) || is_ipmb_bcast_addr(addr)) {
		rv = i_ipmi_req_ipmb(intf, addr, msgid, msg, smi_msg, recv_msg,
				     source_address, source_lun,
				     retries, retry_time_ms);
	} else if (is_lan_addr(addr)) {
		rv = i_ipmi_req_lan(intf, addr, msgid, msg, smi_msg, recv_msg,
				    source_lun, retries, retry_time_ms);
	} else {
	     
		ipmi_inc_stat(intf, sent_invalid_commands);
		rv = -EINVAL;
	}

	if (rv) {
out_err:
		ipmi_free_smi_msg(smi_msg);
		ipmi_free_recv_msg(recv_msg);
	} else {
		ipmi_debug_msg("Send", smi_msg->data, smi_msg->data_size);

		smi_send(intf, intf->handlers, smi_msg, priority);
	}
	rcu_read_unlock();

out:
	return rv;
}
static int deliver_response(struct ipmi_smi *intf, struct ipmi_recv_msg *msg)
{
	int rv = 0;

	if (!msg->user) {
		 
		if (intf->null_user_handler) {
			intf->null_user_handler(intf, msg);
		} else {
			 
			rv = -EINVAL;
		}
		ipmi_free_recv_msg(msg);
	} else if (!oops_in_progress) {
		 
		int index;
		struct ipmi_user *user = acquire_ipmi_user(msg->user, &index);

		if (user) {
			user->handler->ipmi_recv_hndl(msg, user->handler_data);
			release_ipmi_user(user, index);
		} else {
			 
			ipmi_free_recv_msg(msg);
			rv = -EINVAL;
		}
	}

	return rv;
}

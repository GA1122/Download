static void deliver_smi_err_response(struct ipmi_smi *intf,
				     struct ipmi_smi_msg *msg,
				     unsigned char err)
{
	msg->rsp[0] = msg->data[0] | 4;
	msg->rsp[1] = msg->data[1];
	msg->rsp[2] = err;
	msg->rsp_size = 3;
	 
	handle_one_recv_msg(intf, msg);
}

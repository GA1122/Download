void hv_kvp_onchannelcallback(void *context)
{
	struct vmbus_channel *channel = context;
	u32 recvlen;
	u64 requestid;

	struct hv_kvp_msg *kvp_msg;
	struct hv_kvp_msg_enumerate *kvp_data;

	struct icmsg_hdr *icmsghdrp;
	struct icmsg_negotiate *negop = NULL;


	vmbus_recvpacket(channel, recv_buffer, PAGE_SIZE, &recvlen, &requestid);

	if (recvlen > 0) {
		icmsghdrp = (struct icmsg_hdr *)&recv_buffer[
			sizeof(struct vmbuspipe_hdr)];

		if (icmsghdrp->icmsgtype == ICMSGTYPE_NEGOTIATE) {
			vmbus_prep_negotiate_resp(icmsghdrp, negop, recv_buffer);
		} else {
			kvp_msg = (struct hv_kvp_msg *)&recv_buffer[
				sizeof(struct vmbuspipe_hdr) +
				sizeof(struct icmsg_hdr)];

			kvp_data = &kvp_msg->kvp_data;

			 

			if ((kvp_msg->kvp_hdr.pool != KVP_POOL_AUTO) ||
				(kvp_msg->kvp_hdr.operation !=
				KVP_OP_ENUMERATE)) {
				icmsghdrp->status = HV_E_FAIL;
				goto callback_done;
			}

			 
			kvp_transaction.recv_len = recvlen;
			kvp_transaction.recv_channel = channel;
			kvp_transaction.recv_req_id = requestid;
			kvp_transaction.active = true;
			kvp_transaction.index = kvp_data->index;

			 
			schedule_work(&kvp_sendkey_work);
			schedule_delayed_work(&kvp_work, 5*HZ);

			return;

		}

callback_done:

		icmsghdrp->icflags = ICMSGHDRFLAG_TRANSACTION
			| ICMSGHDRFLAG_RESPONSE;

		vmbus_sendpacket(channel, recv_buffer,
				       recvlen, requestid,
				       VM_PKT_DATA_INBAND, 0);
	}

}

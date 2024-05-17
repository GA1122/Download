kvp_cn_callback(struct cn_msg *msg, struct netlink_skb_parms *nsp)
{
	struct hv_ku_msg *message;

	message = (struct hv_ku_msg *)msg->data;
	if (msg->seq == KVP_REGISTER) {
		pr_info("KVP: user-mode registering done.\n");
		kvp_register();
	}

	if (msg->seq == KVP_USER_SET) {
		 
		if (cancel_delayed_work_sync(&kvp_work))
			kvp_respond_to_host(message->kvp_key,
						message->kvp_value,
						!strlen(message->kvp_key));
	}
}

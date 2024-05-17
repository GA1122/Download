static void irda_connect_confirm(void *instance, void *sap,
				 struct qos_info *qos,
				 __u32 max_sdu_size, __u8 max_header_size,
				 struct sk_buff *skb)
{
	struct irda_sock *self;
	struct sock *sk;

	self = instance;

	pr_debug("%s(%p)\n", __func__, self);

	sk = instance;
	if (sk == NULL) {
		dev_kfree_skb(skb);
		return;
	}

	dev_kfree_skb(skb);

	 
	self->max_header_size = max_header_size;

	 
	self->max_sdu_size_tx = max_sdu_size;

	 
	switch (sk->sk_type) {
	case SOCK_STREAM:
		if (max_sdu_size != 0) {
			net_err_ratelimited("%s: max_sdu_size must be 0\n",
					    __func__);
			return;
		}
		self->max_data_size = irttp_get_max_seg_size(self->tsap);
		break;
	case SOCK_SEQPACKET:
		if (max_sdu_size == 0) {
			net_err_ratelimited("%s: max_sdu_size cannot be 0\n",
					    __func__);
			return;
		}
		self->max_data_size = max_sdu_size;
		break;
	default:
		self->max_data_size = irttp_get_max_seg_size(self->tsap);
	}

	pr_debug("%s(), max_data_size=%d\n", __func__,
		 self->max_data_size);

	memcpy(&self->qos_tx, qos, sizeof(struct qos_info));

	 
	sk->sk_state = TCP_ESTABLISHED;
	sk->sk_state_change(sk);
}
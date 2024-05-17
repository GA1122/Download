static void irda_connect_indication(void *instance, void *sap,
				    struct qos_info *qos, __u32 max_sdu_size,
				    __u8 max_header_size, struct sk_buff *skb)
{
	struct irda_sock *self;
	struct sock *sk;

	self = instance;

	IRDA_DEBUG(2, "%s(%p)\n", __func__, self);

	sk = instance;
	if (sk == NULL) {
		dev_kfree_skb(skb);
		return;
	}

	 
	self->max_header_size = max_header_size;

	 
	self->max_sdu_size_tx = max_sdu_size;

	 
	switch (sk->sk_type) {
	case SOCK_STREAM:
		if (max_sdu_size != 0) {
			IRDA_ERROR("%s: max_sdu_size must be 0\n",
				   __func__);
			kfree_skb(skb);
			return;
		}
		self->max_data_size = irttp_get_max_seg_size(self->tsap);
		break;
	case SOCK_SEQPACKET:
		if (max_sdu_size == 0) {
			IRDA_ERROR("%s: max_sdu_size cannot be 0\n",
				   __func__);
			kfree_skb(skb);
			return;
		}
		self->max_data_size = max_sdu_size;
		break;
	default:
		self->max_data_size = irttp_get_max_seg_size(self->tsap);
	}

	IRDA_DEBUG(2, "%s(), max_data_size=%d\n", __func__,
		   self->max_data_size);

	memcpy(&self->qos_tx, qos, sizeof(struct qos_info));

	skb_queue_tail(&sk->sk_receive_queue, skb);
	sk->sk_state_change(sk);
}
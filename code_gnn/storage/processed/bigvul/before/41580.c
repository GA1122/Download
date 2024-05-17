static void irda_disconnect_indication(void *instance, void *sap,
				       LM_REASON reason, struct sk_buff *skb)
{
	struct irda_sock *self;
	struct sock *sk;

	self = instance;

	pr_debug("%s(%p)\n", __func__, self);

	 
	if(skb)
		dev_kfree_skb(skb);

	sk = instance;
	if (sk == NULL) {
		pr_debug("%s(%p) : BUG : sk is NULL\n",
			 __func__, self);
		return;
	}

	 
	bh_lock_sock(sk);
	if (!sock_flag(sk, SOCK_DEAD) && sk->sk_state != TCP_CLOSE) {
		sk->sk_state     = TCP_CLOSE;
		sk->sk_shutdown |= SEND_SHUTDOWN;

		sk->sk_state_change(sk);

		 
		if (self->tsap) {
			irttp_close_tsap(self->tsap);
			self->tsap = NULL;
		}
	}
	bh_unlock_sock(sk);

	 
}

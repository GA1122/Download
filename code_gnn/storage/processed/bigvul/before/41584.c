static void irda_flow_indication(void *instance, void *sap, LOCAL_FLOW flow)
{
	struct irda_sock *self;
	struct sock *sk;

	self = instance;
	sk = instance;
	BUG_ON(sk == NULL);

	switch (flow) {
	case FLOW_STOP:
		pr_debug("%s(), IrTTP wants us to slow down\n",
			 __func__);
		self->tx_flow = flow;
		break;
	case FLOW_START:
		self->tx_flow = flow;
		pr_debug("%s(), IrTTP wants us to start again\n",
			 __func__);
		wake_up_interruptible(sk_sleep(sk));
		break;
	default:
		pr_debug("%s(), Unknown flow command!\n", __func__);
		 
		self->tx_flow = flow;
		break;
	}
}
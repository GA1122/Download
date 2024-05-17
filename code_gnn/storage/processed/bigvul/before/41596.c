static void irda_selective_discovery_indication(discinfo_t *discovery,
						DISCOVERY_MODE mode,
						void *priv)
{
	struct irda_sock *self;

	self = priv;
	if (!self) {
		net_warn_ratelimited("%s: lost myself!\n", __func__);
		return;
	}

	 
	self->cachedaddr = discovery->daddr;

	 
	wake_up_interruptible(&self->query_wait);
}

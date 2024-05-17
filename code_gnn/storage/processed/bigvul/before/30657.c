static void irda_selective_discovery_indication(discinfo_t *discovery,
						DISCOVERY_MODE mode,
						void *priv)
{
	struct irda_sock *self;

	IRDA_DEBUG(2, "%s()\n", __func__);

	self = priv;
	if (!self) {
		IRDA_WARNING("%s: lost myself!\n", __func__);
		return;
	}

	 
	self->cachedaddr = discovery->daddr;

	 
	wake_up_interruptible(&self->query_wait);
}

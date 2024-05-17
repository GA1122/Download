static void irda_discovery_timeout(u_long priv)
{
	struct irda_sock *self;

	IRDA_DEBUG(2, "%s()\n", __func__);

	self = (struct irda_sock *) priv;
	BUG_ON(self == NULL);

	 
	self->cachelog = NULL;
	self->cachedaddr = 0;
	self->errno = -ETIME;

	 
	wake_up_interruptible(&self->query_wait);
}

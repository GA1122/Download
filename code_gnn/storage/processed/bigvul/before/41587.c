static void irda_getvalue_confirm(int result, __u16 obj_id,
				  struct ias_value *value, void *priv)
{
	struct irda_sock *self;

	self = priv;
	if (!self) {
		net_warn_ratelimited("%s: lost myself!\n", __func__);
		return;
	}

	pr_debug("%s(%p)\n", __func__, self);

	 
	iriap_close(self->iriap);
	self->iriap = NULL;

	 
	if (result != IAS_SUCCESS) {
		pr_debug("%s(), IAS query failed! (%d)\n", __func__,
			 result);

		self->errno = result;	 

		 
		wake_up_interruptible(&self->query_wait);

		return;
	}

	 
	self->ias_result = value;
	self->errno = 0;

	 
	wake_up_interruptible(&self->query_wait);
}

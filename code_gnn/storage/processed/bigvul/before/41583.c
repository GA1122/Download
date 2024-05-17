static int irda_find_lsap_sel(struct irda_sock *self, char *name)
{
	pr_debug("%s(%p, %s)\n", __func__, self, name);

	if (self->iriap) {
		net_warn_ratelimited("%s(): busy with a previous query\n",
				     __func__);
		return -EBUSY;
	}

	self->iriap = iriap_open(LSAP_ANY, IAS_CLIENT, self,
				 irda_getvalue_confirm);
	if(self->iriap == NULL)
		return -ENOMEM;

	 
	self->errno = -EHOSTUNREACH;

	 
	iriap_getvaluebyclass_request(self->iriap, self->saddr, self->daddr,
				      name, "IrDA:TinyTP:LsapSel");

	 
	if (wait_event_interruptible(self->query_wait, (self->iriap==NULL)))
		 
		return -EHOSTUNREACH;

	 
	if (self->errno)
	{
		 
		if((self->errno == IAS_CLASS_UNKNOWN) ||
		   (self->errno == IAS_ATTRIB_UNKNOWN))
			return -EADDRNOTAVAIL;
		else
			return -EHOSTUNREACH;
	}

	 
	switch (self->ias_result->type) {
	case IAS_INTEGER:
		pr_debug("%s() int=%d\n",
			 __func__, self->ias_result->t.integer);

		if (self->ias_result->t.integer != -1)
			self->dtsap_sel = self->ias_result->t.integer;
		else
			self->dtsap_sel = 0;
		break;
	default:
		self->dtsap_sel = 0;
		pr_debug("%s(), bad type!\n", __func__);
		break;
	}
	if (self->ias_result)
		irias_delete_value(self->ias_result);

	if (self->dtsap_sel)
		return 0;

	return -EADDRNOTAVAIL;
}

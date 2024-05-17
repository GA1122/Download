static int irda_discover_daddr_and_lsap_sel(struct irda_sock *self, char *name)
{
	discinfo_t *discoveries;	 
	int	number;			 
	int	i;
	int	err = -ENETUNREACH;
	__u32	daddr = DEV_ADDR_ANY;	 
	__u8	dtsap_sel = 0x0;	 

	IRDA_DEBUG(2, "%s(), name=%s\n", __func__, name);

	 
	discoveries = irlmp_get_discoveries(&number, self->mask.word,
					    self->nslots);
	 
	if (discoveries == NULL)
		return -ENETUNREACH;	 

	 
	for(i = 0; i < number; i++) {
		 
		self->daddr = discoveries[i].daddr;
		self->saddr = 0x0;
		IRDA_DEBUG(1, "%s(), trying daddr = %08x\n",
			   __func__, self->daddr);

		 
		err = irda_find_lsap_sel(self, name);
		switch (err) {
		case 0:
			 
			if(daddr != DEV_ADDR_ANY) {
				IRDA_DEBUG(1, "%s(), discovered service ''%s'' in two different devices !!!\n",
					   __func__, name);
				self->daddr = DEV_ADDR_ANY;
				kfree(discoveries);
				return -ENOTUNIQ;
			}
			 
			daddr = self->daddr;
			dtsap_sel = self->dtsap_sel;
			break;
		case -EADDRNOTAVAIL:
			 
			break;
		default:
			 
			IRDA_DEBUG(0, "%s(), unexpected IAS query failure\n", __func__);
			self->daddr = DEV_ADDR_ANY;
			kfree(discoveries);
			return -EHOSTUNREACH;
			break;
		}
	}
	 
	kfree(discoveries);

	 
	if(daddr == DEV_ADDR_ANY) {
		IRDA_DEBUG(1, "%s(), cannot discover service ''%s'' in any device !!!\n",
			   __func__, name);
		self->daddr = DEV_ADDR_ANY;
		return -EADDRNOTAVAIL;
	}

	 
	self->daddr = daddr;
	self->saddr = 0x0;
	self->dtsap_sel = dtsap_sel;

	IRDA_DEBUG(1, "%s(), discovered requested service ''%s'' at address %08x\n",
		   __func__, name, self->daddr);

	return 0;
}

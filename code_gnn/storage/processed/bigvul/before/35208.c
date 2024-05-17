int iriap_getvaluebyclass_request(struct iriap_cb *self,
				  __u32 saddr, __u32 daddr,
				  char *name, char *attr)
{
	struct sk_buff *tx_skb;
	int name_len, attr_len, skb_len;
	__u8 *frame;

	IRDA_ASSERT(self != NULL, return -1;);
	IRDA_ASSERT(self->magic == IAS_MAGIC, return -1;);

	 
	if (!daddr)
		return -1;

	self->daddr = daddr;
	self->saddr = saddr;

	 
	self->operation = GET_VALUE_BY_CLASS;

	 
	iriap_start_watchdog_timer(self, 10*HZ);

	name_len = strlen(name);	 
	attr_len = strlen(attr);	 

	skb_len = self->max_header_size+2+name_len+1+attr_len+4;
	tx_skb = alloc_skb(skb_len, GFP_ATOMIC);
	if (!tx_skb)
		return -ENOMEM;

	 
	skb_reserve(tx_skb, self->max_header_size);
	skb_put(tx_skb, 3+name_len+attr_len);
	frame = tx_skb->data;

	 
	frame[0] = IAP_LST | GET_VALUE_BY_CLASS;
	frame[1] = name_len;                        
	memcpy(frame+2, name, name_len);            
	frame[2+name_len] = attr_len;               
	memcpy(frame+3+name_len, attr, attr_len);   

	iriap_do_client_event(self, IAP_CALL_REQUEST_GVBC, tx_skb);

	 
	dev_kfree_skb(tx_skb);

	return 0;
}

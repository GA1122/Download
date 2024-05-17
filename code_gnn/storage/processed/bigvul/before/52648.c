ppp_receive_mp_frame(struct ppp *ppp, struct sk_buff *skb, struct channel *pch)
{
	u32 mask, seq;
	struct channel *ch;
	int mphdrlen = (ppp->flags & SC_MP_SHORTSEQ)? MPHDRLEN_SSN: MPHDRLEN;

	if (!pskb_may_pull(skb, mphdrlen + 1) || ppp->mrru == 0)
		goto err;		 

	 
	if (ppp->flags & SC_MP_SHORTSEQ) {
		seq = ((skb->data[2] & 0x0f) << 8) | skb->data[3];
		mask = 0xfff;
	} else {
		seq = (skb->data[3] << 16) | (skb->data[4] << 8)| skb->data[5];
		mask = 0xffffff;
	}
	PPP_MP_CB(skb)->BEbits = skb->data[2];
	skb_pull(skb, mphdrlen);	 

	 
	if ((PPP_MP_CB(skb)->BEbits & B) && (skb->data[0] & 1))
		*skb_push(skb, 1) = 0;

	 
	seq |= ppp->minseq & ~mask;
	if ((int)(ppp->minseq - seq) > (int)(mask >> 1))
		seq += mask + 1;
	else if ((int)(seq - ppp->minseq) > (int)(mask >> 1))
		seq -= mask + 1;	 
	PPP_MP_CB(skb)->sequence = seq;
	pch->lastseq = seq;

	 
	if (seq_before(seq, ppp->nextseq)) {
		kfree_skb(skb);
		++ppp->dev->stats.rx_dropped;
		ppp_receive_error(ppp);
		return;
	}

	 
	list_for_each_entry(ch, &ppp->channels, clist) {
		if (seq_before(ch->lastseq, seq))
			seq = ch->lastseq;
	}
	if (seq_before(ppp->minseq, seq))
		ppp->minseq = seq;

	 
	ppp_mp_insert(ppp, skb);

	 
	if (skb_queue_len(&ppp->mrq) >= PPP_MP_MAX_QLEN) {
		struct sk_buff *mskb = skb_peek(&ppp->mrq);
		if (seq_before(ppp->minseq, PPP_MP_CB(mskb)->sequence))
			ppp->minseq = PPP_MP_CB(mskb)->sequence;
	}

	 
	while ((skb = ppp_mp_reconstruct(ppp))) {
		if (pskb_may_pull(skb, 2))
			ppp_receive_nonmp_frame(ppp, skb);
		else {
			++ppp->dev->stats.rx_length_errors;
			kfree_skb(skb);
			ppp_receive_error(ppp);
		}
	}

	return;

 err:
	kfree_skb(skb);
	ppp_receive_error(ppp);
}

static int ppp_mp_explode(struct ppp *ppp, struct sk_buff *skb)
{
	int len, totlen;
	int i, bits, hdrlen, mtu;
	int flen;
	int navail, nfree, nzero;
	int nbigger;
	int totspeed;
	int totfree;
	unsigned char *p, *q;
	struct list_head *list;
	struct channel *pch;
	struct sk_buff *frag;
	struct ppp_channel *chan;

	totspeed = 0;  
	nfree = 0;  
	navail = 0;  
	nzero = 0;  
	totfree = 0;  

	hdrlen = (ppp->flags & SC_MP_XSHORTSEQ)? MPHDRLEN_SSN: MPHDRLEN;
	i = 0;
	list_for_each_entry(pch, &ppp->channels, clist) {
		if (pch->chan) {
			pch->avail = 1;
			navail++;
			pch->speed = pch->chan->speed;
		} else {
			pch->avail = 0;
		}
		if (pch->avail) {
			if (skb_queue_empty(&pch->file.xq) ||
				!pch->had_frag) {
					if (pch->speed == 0)
						nzero++;
					else
						totspeed += pch->speed;

					pch->avail = 2;
					++nfree;
					++totfree;
				}
			if (!pch->had_frag && i < ppp->nxchan)
				ppp->nxchan = i;
		}
		++i;
	}
	 
	if (nfree == 0 || nfree < navail / 2)
		return 0;  

	 
	p = skb->data;
	len = skb->len;
	if (*p == 0 && mp_protocol_compress) {
		++p;
		--len;
	}

	totlen = len;
	nbigger = len % nfree;

	 
	list = &ppp->channels;
	for (i = 0; i < ppp->nxchan; ++i) {
		list = list->next;
		if (list == &ppp->channels) {
			i = 0;
			break;
		}
	}

	 
	bits = B;
	while (len > 0) {
		list = list->next;
		if (list == &ppp->channels) {
			i = 0;
			continue;
		}
		pch = list_entry(list, struct channel, clist);
		++i;
		if (!pch->avail)
			continue;

		 
		if (pch->avail == 1) {
			if (nfree > 0)
				continue;
		} else {
			pch->avail = 1;
		}

		 
		spin_lock_bh(&pch->downl);
		if (pch->chan == NULL) {
			 
			if (pch->speed == 0)
				nzero--;
			else
				totspeed -= pch->speed;

			spin_unlock_bh(&pch->downl);
			pch->avail = 0;
			totlen = len;
			totfree--;
			nfree--;
			if (--navail == 0)
				break;
			continue;
		}

		 
		flen = len;
		if (nfree > 0) {
			if (pch->speed == 0) {
				flen = len/nfree;
				if (nbigger > 0) {
					flen++;
					nbigger--;
				}
			} else {
				flen = (((totfree - nzero)*(totlen + hdrlen*totfree)) /
					((totspeed*totfree)/pch->speed)) - hdrlen;
				if (nbigger > 0) {
					flen += ((totfree - nzero)*pch->speed)/totspeed;
					nbigger -= ((totfree - nzero)*pch->speed)/
							totspeed;
				}
			}
			nfree--;
		}

		 
		if ((nfree <= 0) || (flen > len))
			flen = len;
		 
		if (flen <= 0) {
			pch->avail = 2;
			spin_unlock_bh(&pch->downl);
			continue;
		}

		 
		mtu = pch->chan->mtu - (hdrlen - 2);
		if (mtu < 4)
			mtu = 4;
		if (flen > mtu)
			flen = mtu;
		if (flen == len)
			bits |= E;
		frag = alloc_skb(flen + hdrlen + (flen == 0), GFP_ATOMIC);
		if (!frag)
			goto noskb;
		q = skb_put(frag, flen + hdrlen);

		 
		put_unaligned_be16(PPP_MP, q);
		if (ppp->flags & SC_MP_XSHORTSEQ) {
			q[2] = bits + ((ppp->nxseq >> 8) & 0xf);
			q[3] = ppp->nxseq;
		} else {
			q[2] = bits;
			q[3] = ppp->nxseq >> 16;
			q[4] = ppp->nxseq >> 8;
			q[5] = ppp->nxseq;
		}

		memcpy(q + hdrlen, p, flen);

		 
		chan = pch->chan;
		if (!skb_queue_empty(&pch->file.xq) ||
			!chan->ops->start_xmit(chan, frag))
			skb_queue_tail(&pch->file.xq, frag);
		pch->had_frag = 1;
		p += flen;
		len -= flen;
		++ppp->nxseq;
		bits = 0;
		spin_unlock_bh(&pch->downl);
	}
	ppp->nxchan = i;

	return 1;

 noskb:
	spin_unlock_bh(&pch->downl);
	if (ppp->debug & 1)
		netdev_err(ppp->dev, "PPP: no memory (fragment)\n");
	++ppp->dev->stats.tx_errors;
	++ppp->nxseq;
	return 1;	 
}

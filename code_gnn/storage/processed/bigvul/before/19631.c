static int nf_ct_frag6_queue(struct nf_ct_frag6_queue *fq, struct sk_buff *skb,
			     const struct frag_hdr *fhdr, int nhoff)
{
	struct sk_buff *prev, *next;
	int offset, end;

	if (fq->q.last_in & INET_FRAG_COMPLETE) {
		pr_debug("Allready completed\n");
		goto err;
	}

	offset = ntohs(fhdr->frag_off) & ~0x7;
	end = offset + (ntohs(ipv6_hdr(skb)->payload_len) -
			((u8 *)(fhdr + 1) - (u8 *)(ipv6_hdr(skb) + 1)));

	if ((unsigned int)end > IPV6_MAXPLEN) {
		pr_debug("offset is too large.\n");
		return -1;
	}

	if (skb->ip_summed == CHECKSUM_COMPLETE) {
		const unsigned char *nh = skb_network_header(skb);
		skb->csum = csum_sub(skb->csum,
				     csum_partial(nh, (u8 *)(fhdr + 1) - nh,
						  0));
	}

	 
	if (!(fhdr->frag_off & htons(IP6_MF))) {
		 
		if (end < fq->q.len ||
		    ((fq->q.last_in & INET_FRAG_LAST_IN) && end != fq->q.len)) {
			pr_debug("already received last fragment\n");
			goto err;
		}
		fq->q.last_in |= INET_FRAG_LAST_IN;
		fq->q.len = end;
	} else {
		 
		if (end & 0x7) {
			 
			pr_debug("end of fragment not rounded to 8 bytes.\n");
			return -1;
		}
		if (end > fq->q.len) {
			 
			if (fq->q.last_in & INET_FRAG_LAST_IN) {
				pr_debug("last packet already reached.\n");
				goto err;
			}
			fq->q.len = end;
		}
	}

	if (end == offset)
		goto err;

	 
	if (!pskb_pull(skb, (u8 *) (fhdr + 1) - skb->data)) {
		pr_debug("queue: message is too short.\n");
		goto err;
	}
	if (pskb_trim_rcsum(skb, end - offset)) {
		pr_debug("Can't trim\n");
		goto err;
	}

	 
	prev = NULL;
	for (next = fq->q.fragments; next != NULL; next = next->next) {
		if (NFCT_FRAG6_CB(next)->offset >= offset)
			break;	 
		prev = next;
	}

	 
	if (prev) {
		int i = (NFCT_FRAG6_CB(prev)->offset + prev->len) - offset;

		if (i > 0) {
			offset += i;
			if (end <= offset) {
				pr_debug("overlap\n");
				goto err;
			}
			if (!pskb_pull(skb, i)) {
				pr_debug("Can't pull\n");
				goto err;
			}
			if (skb->ip_summed != CHECKSUM_UNNECESSARY)
				skb->ip_summed = CHECKSUM_NONE;
		}
	}

	 
	while (next && NFCT_FRAG6_CB(next)->offset < end) {
		 
		int i = end - NFCT_FRAG6_CB(next)->offset;

		if (i < next->len) {
			 
			pr_debug("Eat head of the overlapped parts.: %d", i);
			if (!pskb_pull(next, i))
				goto err;

			 
			NFCT_FRAG6_CB(next)->offset += i;
			fq->q.meat -= i;
			if (next->ip_summed != CHECKSUM_UNNECESSARY)
				next->ip_summed = CHECKSUM_NONE;
			break;
		} else {
			struct sk_buff *free_it = next;

			 
			next = next->next;

			if (prev)
				prev->next = next;
			else
				fq->q.fragments = next;

			fq->q.meat -= free_it->len;
			frag_kfree_skb(free_it, NULL);
		}
	}

	NFCT_FRAG6_CB(skb)->offset = offset;

	 
	skb->next = next;
	if (prev)
		prev->next = skb;
	else
		fq->q.fragments = skb;

	skb->dev = NULL;
	fq->q.stamp = skb->tstamp;
	fq->q.meat += skb->len;
	atomic_add(skb->truesize, &nf_init_frags.mem);

	 
	if (offset == 0) {
		fq->nhoffset = nhoff;
		fq->q.last_in |= INET_FRAG_FIRST_IN;
	}
	write_lock(&nf_frags.lock);
	list_move_tail(&fq->q.lru_list, &nf_init_frags.lru_list);
	write_unlock(&nf_frags.lock);
	return 0;

err:
	return -1;
}

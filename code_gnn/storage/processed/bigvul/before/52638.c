ppp_mp_reconstruct(struct ppp *ppp)
{
	u32 seq = ppp->nextseq;
	u32 minseq = ppp->minseq;
	struct sk_buff_head *list = &ppp->mrq;
	struct sk_buff *p, *tmp;
	struct sk_buff *head, *tail;
	struct sk_buff *skb = NULL;
	int lost = 0, len = 0;

	if (ppp->mrru == 0)	 
		return NULL;
	head = list->next;
	tail = NULL;
	skb_queue_walk_safe(list, p, tmp) {
	again:
		if (seq_before(PPP_MP_CB(p)->sequence, seq)) {
			 
			netdev_err(ppp->dev, "ppp_mp_reconstruct bad "
				   "seq %u < %u\n",
				   PPP_MP_CB(p)->sequence, seq);
			__skb_unlink(p, list);
			kfree_skb(p);
			continue;
		}
		if (PPP_MP_CB(p)->sequence != seq) {
			u32 oldseq;
			 
			if (seq_after(seq, minseq))
				break;
			 
			lost = 1;
			oldseq = seq;
			seq = seq_before(minseq, PPP_MP_CB(p)->sequence)?
				minseq + 1: PPP_MP_CB(p)->sequence;

			if (ppp->debug & 1)
				netdev_printk(KERN_DEBUG, ppp->dev,
					      "lost frag %u..%u\n",
					      oldseq, seq-1);

			goto again;
		}

		 

		 
		if (PPP_MP_CB(p)->BEbits & B) {
			head = p;
			lost = 0;
			len = 0;
		}

		len += p->len;

		 
		if (lost == 0 && (PPP_MP_CB(p)->BEbits & E) &&
		    (PPP_MP_CB(head)->BEbits & B)) {
			if (len > ppp->mrru + 2) {
				++ppp->dev->stats.rx_length_errors;
				netdev_printk(KERN_DEBUG, ppp->dev,
					      "PPP: reconstructed packet"
					      " is too long (%d)\n", len);
			} else {
				tail = p;
				break;
			}
			ppp->nextseq = seq + 1;
		}

		 
		if (PPP_MP_CB(p)->BEbits & E) {
			struct sk_buff *tmp2;

			skb_queue_reverse_walk_from_safe(list, p, tmp2) {
				if (ppp->debug & 1)
					netdev_printk(KERN_DEBUG, ppp->dev,
						      "discarding frag %u\n",
						      PPP_MP_CB(p)->sequence);
				__skb_unlink(p, list);
				kfree_skb(p);
			}
			head = skb_peek(list);
			if (!head)
				break;
		}
		++seq;
	}

	 
	if (tail != NULL) {
		 
		if (PPP_MP_CB(head)->sequence != ppp->nextseq) {
			skb_queue_walk_safe(list, p, tmp) {
				if (p == head)
					break;
				if (ppp->debug & 1)
					netdev_printk(KERN_DEBUG, ppp->dev,
						      "discarding frag %u\n",
						      PPP_MP_CB(p)->sequence);
				__skb_unlink(p, list);
				kfree_skb(p);
			}

			if (ppp->debug & 1)
				netdev_printk(KERN_DEBUG, ppp->dev,
					      "  missed pkts %u..%u\n",
					      ppp->nextseq,
					      PPP_MP_CB(head)->sequence-1);
			++ppp->dev->stats.rx_dropped;
			ppp_receive_error(ppp);
		}

		skb = head;
		if (head != tail) {
			struct sk_buff **fragpp = &skb_shinfo(skb)->frag_list;
			p = skb_queue_next(list, head);
			__skb_unlink(skb, list);
			skb_queue_walk_from_safe(list, p, tmp) {
				__skb_unlink(p, list);
				*fragpp = p;
				p->next = NULL;
				fragpp = &p->next;

				skb->len += p->len;
				skb->data_len += p->len;
				skb->truesize += p->truesize;

				if (p == tail)
					break;
			}
		} else {
			__skb_unlink(skb, list);
		}

		ppp->nextseq = PPP_MP_CB(tail)->sequence + 1;
	}

	return skb;
}

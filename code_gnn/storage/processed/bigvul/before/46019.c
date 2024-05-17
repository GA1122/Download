static bool batadv_frag_insert_packet(struct batadv_orig_node *orig_node,
				      struct sk_buff *skb,
				      struct hlist_head *chain_out)
{
	struct batadv_frag_table_entry *chain;
	struct batadv_frag_list_entry *frag_entry_new = NULL, *frag_entry_curr;
	struct batadv_frag_list_entry *frag_entry_last = NULL;
	struct batadv_frag_packet *frag_packet;
	uint8_t bucket;
	uint16_t seqno, hdr_size = sizeof(struct batadv_frag_packet);
	bool ret = false;

	 
	if (skb_linearize(skb) < 0)
		goto err;

	frag_packet = (struct batadv_frag_packet *)skb->data;
	seqno = ntohs(frag_packet->seqno);
	bucket = seqno % BATADV_FRAG_BUFFER_COUNT;

	frag_entry_new = kmalloc(sizeof(*frag_entry_new), GFP_ATOMIC);
	if (!frag_entry_new)
		goto err;

	frag_entry_new->skb = skb;
	frag_entry_new->no = frag_packet->no;

	 
	chain = &orig_node->fragments[bucket];
	spin_lock_bh(&chain->lock);
	if (batadv_frag_init_chain(chain, seqno)) {
		hlist_add_head(&frag_entry_new->list, &chain->head);
		chain->size = skb->len - hdr_size;
		chain->timestamp = jiffies;
		ret = true;
		goto out;
	}

	 
	hlist_for_each_entry(frag_entry_curr, &chain->head, list) {
		 
		if (frag_entry_curr->no == frag_entry_new->no)
			goto err_unlock;

		 
		if (frag_entry_curr->no < frag_entry_new->no) {
			hlist_add_before(&frag_entry_new->list,
					 &frag_entry_curr->list);
			chain->size += skb->len - hdr_size;
			chain->timestamp = jiffies;
			ret = true;
			goto out;
		}

		 
		frag_entry_last = frag_entry_curr;
	}

	 
	if (likely(frag_entry_last)) {
		hlist_add_behind(&frag_entry_new->list, &frag_entry_last->list);
		chain->size += skb->len - hdr_size;
		chain->timestamp = jiffies;
		ret = true;
	}

out:
	if (chain->size > batadv_frag_size_limit() ||
	    ntohs(frag_packet->total_size) > batadv_frag_size_limit()) {
		 
		batadv_frag_clear_chain(&chain->head);
		chain->size = 0;
	} else if (ntohs(frag_packet->total_size) == chain->size) {
		 
		hlist_move_list(&chain->head, chain_out);
		chain->size = 0;
	}

err_unlock:
	spin_unlock_bh(&chain->lock);

err:
	if (!ret)
		kfree(frag_entry_new);

	return ret;
}
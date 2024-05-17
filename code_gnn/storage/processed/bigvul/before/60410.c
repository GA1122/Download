static void *__packet_lookup_frame_in_block(struct packet_sock *po,
					    struct sk_buff *skb,
						int status,
					    unsigned int len
					    )
{
	struct tpacket_kbdq_core *pkc;
	struct tpacket_block_desc *pbd;
	char *curr, *end;

	pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
	pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	 
	if (prb_queue_frozen(pkc)) {
		 
		if (prb_curr_blk_in_use(pbd)) {
			 
			return NULL;
		} else {
			 
			prb_open_block(pkc, pbd);
		}
	}

	smp_mb();
	curr = pkc->nxt_offset;
	pkc->skb = skb;
	end = (char *)pbd + pkc->kblk_size;

	 
	if (curr+TOTAL_PKT_LEN_INCL_ALIGN(len) < end) {
		prb_fill_curr_block(curr, pkc, pbd, len);
		return (void *)curr;
	}

	 
	prb_retire_current_block(pkc, po, 0);

	 
	curr = (char *)prb_dispatch_next_block(pkc, po);
	if (curr) {
		pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);
		prb_fill_curr_block(curr, pkc, pbd, len);
		return (void *)curr;
	}

	 
	return NULL;
}

static void prb_retire_rx_blk_timer_expired(unsigned long data)
{
	struct packet_sock *po = (struct packet_sock *)data;
	struct tpacket_kbdq_core *pkc = GET_PBDQC_FROM_RB(&po->rx_ring);
	unsigned int frozen;
	struct tpacket_block_desc *pbd;

	spin_lock(&po->sk.sk_receive_queue.lock);

	frozen = prb_queue_frozen(pkc);
	pbd = GET_CURR_PBLOCK_DESC_FROM_CORE(pkc);

	if (unlikely(pkc->delete_blk_timer))
		goto out;

	 
	if (BLOCK_NUM_PKTS(pbd)) {
		while (atomic_read(&pkc->blk_fill_in_prog)) {
			 
			cpu_relax();
		}
	}

	if (pkc->last_kactive_blk_num == pkc->kactive_blk_num) {
		if (!frozen) {
			if (!BLOCK_NUM_PKTS(pbd)) {
				 
				goto refresh_timer;
			}
			prb_retire_current_block(pkc, po, TP_STATUS_BLK_TMO);
			if (!prb_dispatch_next_block(pkc, po))
				goto refresh_timer;
			else
				goto out;
		} else {
			 
			if (prb_curr_blk_in_use(pbd)) {
				 
				goto refresh_timer;
			} else {
			        
				prb_open_block(pkc, pbd);
				goto out;
			}
		}
	}

refresh_timer:
	_prb_refresh_rx_retire_blk_timer(pkc);

out:
	spin_unlock(&po->sk.sk_receive_queue.lock);
}

static int tg3_poll_msix(struct napi_struct *napi, int budget)
{
	struct tg3_napi *tnapi = container_of(napi, struct tg3_napi, napi);
	struct tg3 *tp = tnapi->tp;
	int work_done = 0;
	struct tg3_hw_status *sblk = tnapi->hw_status;

	while (1) {
		work_done = tg3_poll_work(tnapi, work_done, budget);

		if (unlikely(tg3_flag(tp, TX_RECOVERY_PENDING)))
			goto tx_recovery;

		if (unlikely(work_done >= budget))
			break;

		 
		tnapi->last_tag = sblk->status_tag;
		tnapi->last_irq_tag = tnapi->last_tag;
		rmb();

		 
		if (likely(sblk->idx[0].tx_consumer == tnapi->tx_cons &&
			   *(tnapi->rx_rcb_prod_idx) == tnapi->rx_rcb_ptr)) {

			 
			if (tnapi == &tp->napi[1] && tp->rx_refill)
				continue;

			napi_complete(napi);
			 
			tw32_mailbox(tnapi->int_mbox, tnapi->last_tag << 24);

			 
			if (unlikely(tnapi == &tp->napi[1] && tp->rx_refill)) {
				tw32(HOSTCC_MODE, tp->coalesce_mode |
						  HOSTCC_MODE_ENABLE |
						  tnapi->coal_now);
			}
			mmiowb();
			break;
		}
	}

	return work_done;

tx_recovery:
	 
	napi_complete(napi);
	tg3_reset_task_schedule(tp);
	return work_done;
}

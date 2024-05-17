static inline void tg3_netif_start(struct tg3 *tp)
{
	tg3_ptp_resume(tp);

	 
	netif_tx_wake_all_queues(tp->dev);

	if (tp->link_up)
		netif_carrier_on(tp->dev);

	tg3_napi_enable(tp);
	tp->napi[0].hw_status->status |= SD_STATUS_UPDATED;
	tg3_enable_ints(tp);
}

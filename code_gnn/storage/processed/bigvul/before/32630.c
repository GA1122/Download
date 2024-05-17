static inline void tg3_netif_stop(struct tg3 *tp)
{
	tp->dev->trans_start = jiffies;	 
	tg3_napi_disable(tp);
	netif_carrier_off(tp->dev);
	netif_tx_disable(tp->dev);
}

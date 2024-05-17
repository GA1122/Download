void __ieee80211_tx_skb_tid_band(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb, int tid,
				 enum ieee80211_band band)
{
	int ac = ieee802_1d_to_ac[tid & 7];

	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, 0);
	skb_set_transport_header(skb, 0);

	skb_set_queue_mapping(skb, ac);
	skb->priority = tid;

	skb->dev = sdata->dev;

	 
	local_bh_disable();
	ieee80211_xmit(sdata, skb, band);
	local_bh_enable();
}

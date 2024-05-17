static void __ieee80211_beacon_add_tim(struct ieee80211_sub_if_data *sdata,
				       struct ps_data *ps, struct sk_buff *skb)
{
	u8 *pos, *tim;
	int aid0 = 0;
	int i, have_bits = 0, n1, n2;

	 
	if (atomic_read(&ps->num_sta_ps) > 0)
		 
		have_bits = !bitmap_empty((unsigned long *)ps->tim,
					  IEEE80211_MAX_AID+1);

	if (ps->dtim_count == 0)
		ps->dtim_count = sdata->vif.bss_conf.dtim_period - 1;
	else
		ps->dtim_count--;

	tim = pos = (u8 *) skb_put(skb, 6);
	*pos++ = WLAN_EID_TIM;
	*pos++ = 4;
	*pos++ = ps->dtim_count;
	*pos++ = sdata->vif.bss_conf.dtim_period;

	if (ps->dtim_count == 0 && !skb_queue_empty(&ps->bc_buf))
		aid0 = 1;

	ps->dtim_bc_mc = aid0 == 1;

	if (have_bits) {
		 
		n1 = 0;
		for (i = 0; i < IEEE80211_MAX_TIM_LEN; i++) {
			if (ps->tim[i]) {
				n1 = i & 0xfe;
				break;
			}
		}
		n2 = n1;
		for (i = IEEE80211_MAX_TIM_LEN - 1; i >= n1; i--) {
			if (ps->tim[i]) {
				n2 = i;
				break;
			}
		}

		 
		*pos++ = n1 | aid0;
		 
		skb_put(skb, n2 - n1);
		memcpy(pos, ps->tim + n1, n2 - n1 + 1);

		tim[1] = n2 - n1 + 4;
	} else {
		*pos++ = aid0;  
		*pos++ = 0;  
	}
}
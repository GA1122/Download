static void mac80211_hwsim_add_vendor_rtap(struct sk_buff *skb)
{
	 
#ifdef HWSIM_RADIOTAP_OUI
	struct ieee80211_vendor_radiotap *rtap;

	 
	rtap = skb_push(skb, sizeof(*rtap) + 8 + 4);
	rtap->oui[0] = HWSIM_RADIOTAP_OUI[0];
	rtap->oui[1] = HWSIM_RADIOTAP_OUI[1];
	rtap->oui[2] = HWSIM_RADIOTAP_OUI[2];
	rtap->subns = 127;

	 
	rtap->present = BIT(0);
	 
	rtap->len = 8;
	 
	rtap->align = 8;
	 
	rtap->pad = 4;
	 
	memcpy(rtap->data, "ABCDEFGH", 8);
	 
	memset(rtap->data + 8, 0, 4);

	IEEE80211_SKB_RXCB(skb)->flag |= RX_FLAG_RADIOTAP_VENDOR_DATA;
#endif
}

int hostap_80211_get_hdrlen(__le16 fc)
{
	if (ieee80211_is_data(fc) && ieee80211_has_a4 (fc))
		return 30;  
	else if (ieee80211_is_cts(fc) || ieee80211_is_ack(fc))
		return 10;
	else if (ieee80211_is_ctl(fc))
		return 16;

	return 24;
}
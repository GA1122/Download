static bool nl80211_can_set_dev_channel(struct wireless_dev *wdev)
{
	 
	return !wdev ||
		wdev->iftype == NL80211_IFTYPE_AP ||
		wdev->iftype == NL80211_IFTYPE_WDS ||
		wdev->iftype == NL80211_IFTYPE_MESH_POINT ||
		wdev->iftype == NL80211_IFTYPE_MONITOR ||
		wdev->iftype == NL80211_IFTYPE_P2P_GO;
}

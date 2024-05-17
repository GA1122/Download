static int mac80211_hwsim_change_interface(struct ieee80211_hw *hw,
					   struct ieee80211_vif *vif,
					   enum nl80211_iftype newtype,
					   bool newp2p)
{
	newtype = ieee80211_iftype_p2p(newtype, newp2p);
	wiphy_dbg(hw->wiphy,
		  "%s (old type=%d, new type=%d, mac_addr=%pM)\n",
		  __func__, ieee80211_vif_type_p2p(vif),
		    newtype, vif->addr);
	hwsim_check_magic(vif);

	 
	vif->cab_queue = 0;

	return 0;
}

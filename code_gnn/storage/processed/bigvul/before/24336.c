static void ieee80211_setup_sdata(struct ieee80211_sub_if_data *sdata,
				  enum nl80211_iftype type)
{
	 
	memset(&sdata->u, 0, sizeof(sdata->u));

	 
	sdata->vif.type = type;
	sdata->vif.p2p = false;
	sdata->dev->netdev_ops = &ieee80211_dataif_ops;
	sdata->wdev.iftype = type;

	sdata->control_port_protocol = cpu_to_be16(ETH_P_PAE);
	sdata->control_port_no_encrypt = false;

	 
	sdata->dev->type = ARPHRD_ETHER;

	skb_queue_head_init(&sdata->skb_queue);
	INIT_WORK(&sdata->work, ieee80211_iface_work);

	switch (type) {
	case NL80211_IFTYPE_P2P_GO:
		type = NL80211_IFTYPE_AP;
		sdata->vif.type = type;
		sdata->vif.p2p = true;
		 
	case NL80211_IFTYPE_AP:
		skb_queue_head_init(&sdata->u.ap.ps_bc_buf);
		INIT_LIST_HEAD(&sdata->u.ap.vlans);
		break;
	case NL80211_IFTYPE_P2P_CLIENT:
		type = NL80211_IFTYPE_STATION;
		sdata->vif.type = type;
		sdata->vif.p2p = true;
		 
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_setup_sdata(sdata);
		break;
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_setup_sdata(sdata);
		break;
	case NL80211_IFTYPE_MESH_POINT:
		if (ieee80211_vif_is_mesh(&sdata->vif))
			ieee80211_mesh_init_sdata(sdata);
		break;
	case NL80211_IFTYPE_MONITOR:
		sdata->dev->type = ARPHRD_IEEE80211_RADIOTAP;
		sdata->dev->netdev_ops = &ieee80211_monitorif_ops;
		sdata->u.mntr_flags = MONITOR_FLAG_CONTROL |
				      MONITOR_FLAG_OTHER_BSS;
		break;
	case NL80211_IFTYPE_WDS:
	case NL80211_IFTYPE_AP_VLAN:
		break;
	case NL80211_IFTYPE_UNSPECIFIED:
	case NUM_NL80211_IFTYPES:
		BUG();
		break;
	}

	ieee80211_debugfs_add_netdev(sdata);
}
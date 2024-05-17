static int nl80211_join_mesh(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct mesh_config cfg;
	struct mesh_setup setup;
	int err;

	 
	memcpy(&cfg, &default_mesh_config, sizeof(cfg));
	memcpy(&setup, &default_mesh_setup, sizeof(setup));

	if (info->attrs[NL80211_ATTR_MESH_CONFIG]) {
		 
		err = nl80211_parse_mesh_config(info, &cfg, NULL);
		if (err)
			return err;
	}

	if (!info->attrs[NL80211_ATTR_MESH_ID] ||
	    !nla_len(info->attrs[NL80211_ATTR_MESH_ID]))
		return -EINVAL;

	setup.mesh_id = nla_data(info->attrs[NL80211_ATTR_MESH_ID]);
	setup.mesh_id_len = nla_len(info->attrs[NL80211_ATTR_MESH_ID]);

	if (info->attrs[NL80211_ATTR_MESH_SETUP]) {
		 
		err = nl80211_parse_mesh_setup(info, &setup);
		if (err)
			return err;
	}

	return cfg80211_join_mesh(rdev, dev, &setup, &cfg);
}
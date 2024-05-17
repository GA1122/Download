static int setup_proc_entry( struct net_device *dev,
			     struct airo_info *apriv ) {
	struct proc_dir_entry *entry;
	 
	strcpy(apriv->proc_name,dev->name);
	apriv->proc_entry = proc_mkdir_mode(apriv->proc_name, airo_perm,
					    airo_entry);
	if (!apriv->proc_entry)
		goto fail;
	apriv->proc_entry->uid = proc_uid;
	apriv->proc_entry->gid = proc_gid;

	 
	entry = proc_create_data("StatsDelta", S_IRUGO & proc_perm,
				 apriv->proc_entry, &proc_statsdelta_ops, dev);
	if (!entry)
		goto fail_stats_delta;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("Stats", S_IRUGO & proc_perm,
				 apriv->proc_entry, &proc_stats_ops, dev);
	if (!entry)
		goto fail_stats;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("Status", S_IRUGO & proc_perm,
				 apriv->proc_entry, &proc_status_ops, dev);
	if (!entry)
		goto fail_status;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("Config", proc_perm,
				 apriv->proc_entry, &proc_config_ops, dev);
	if (!entry)
		goto fail_config;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("SSID", proc_perm,
				 apriv->proc_entry, &proc_SSID_ops, dev);
	if (!entry)
		goto fail_ssid;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("APList", proc_perm,
				 apriv->proc_entry, &proc_APList_ops, dev);
	if (!entry)
		goto fail_aplist;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("BSSList", proc_perm,
				 apriv->proc_entry, &proc_BSSList_ops, dev);
	if (!entry)
		goto fail_bsslist;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	 
	entry = proc_create_data("WepKey", proc_perm,
				 apriv->proc_entry, &proc_wepkey_ops, dev);
	if (!entry)
		goto fail_wepkey;
	entry->uid = proc_uid;
	entry->gid = proc_gid;

	return 0;

fail_wepkey:
	remove_proc_entry("BSSList", apriv->proc_entry);
fail_bsslist:
	remove_proc_entry("APList", apriv->proc_entry);
fail_aplist:
	remove_proc_entry("SSID", apriv->proc_entry);
fail_ssid:
	remove_proc_entry("Config", apriv->proc_entry);
fail_config:
	remove_proc_entry("Status", apriv->proc_entry);
fail_status:
	remove_proc_entry("Stats", apriv->proc_entry);
fail_stats:
	remove_proc_entry("StatsDelta", apriv->proc_entry);
fail_stats_delta:
	remove_proc_entry(apriv->proc_name, airo_entry);
fail:
	return -ENOMEM;
}
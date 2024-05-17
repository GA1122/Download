init_vrrp_keywords(bool active)
{
	 
	install_keyword_root("track_group", &static_track_group_handler, active);
	install_keyword("group", &static_track_group_group_handler);
	install_keyword_root("static_ipaddress", &static_addresses_handler, active);
#ifdef _HAVE_FIB_ROUTING_
	install_keyword_root("static_routes", &static_routes_handler, active);
	install_keyword_root("static_rules", &static_rules_handler, active);
#endif

	 
	install_keyword_root("vrrp_sync_group", &vrrp_sync_group_handler, active);
	install_keyword("group", &vrrp_group_handler);
	install_keyword("track_interface", &vrrp_group_track_if_handler);
	install_keyword("track_script", &vrrp_group_track_scr_handler);
	install_keyword("track_file", &vrrp_group_track_file_handler);
#ifdef _WITH_BFD_
	install_keyword("track_bfd", &vrrp_group_track_bfd_handler);
#endif
	install_keyword("notify_backup", &vrrp_gnotify_backup_handler);
	install_keyword("notify_master", &vrrp_gnotify_master_handler);
	install_keyword("notify_fault", &vrrp_gnotify_fault_handler);
	install_keyword("notify_stop", &vrrp_gnotify_stop_handler);
	install_keyword("notify", &vrrp_gnotify_handler);
	install_keyword("smtp_alert", &vrrp_gsmtp_handler);
	install_keyword("global_tracking", &vrrp_gglobal_tracking_handler);
	install_keyword("sync_group_tracking_weight", &vrrp_sg_tracking_weight_handler);

	install_keyword_root("garp_group", &garp_group_handler, active);
	install_keyword("garp_interval", &garp_group_garp_interval_handler);
	install_keyword("gna_interval", &garp_group_gna_interval_handler);
	install_keyword("interface", &garp_group_interface_handler);
	install_keyword("interfaces", &garp_group_interfaces_handler);
	install_sublevel_end_handler(&garp_group_end_handler);

	 
	install_keyword_root("vrrp_instance", &vrrp_handler, active);
#ifdef _HAVE_VRRP_VMAC_
	install_keyword("use_vmac", &vrrp_vmac_handler);
	install_keyword("vmac_xmit_base", &vrrp_vmac_xmit_base_handler);
#endif
	install_keyword("unicast_peer", &vrrp_unicast_peer_handler);
#ifdef _WITH_UNICAST_CHKSUM_COMPAT_
	install_keyword("old_unicast_checksum", &vrrp_unicast_chksum_handler);
#endif
	install_keyword("native_ipv6", &vrrp_native_ipv6_handler);
	install_keyword("state", &vrrp_state_handler);
	install_keyword("interface", &vrrp_int_handler);
	install_keyword("dont_track_primary", &vrrp_dont_track_handler);
	install_keyword("track_interface", &vrrp_track_if_handler);
	install_keyword("track_script", &vrrp_track_scr_handler);
	install_keyword("track_file", &vrrp_track_file_handler);
#ifdef _WITH_BFD_
	install_keyword("track_bfd", &vrrp_track_bfd_handler);
#endif
	install_keyword("mcast_src_ip", &vrrp_srcip_handler);
	install_keyword("unicast_src_ip", &vrrp_srcip_handler);
	install_keyword("track_src_ip", &vrrp_track_srcip_handler);
	install_keyword("virtual_router_id", &vrrp_vrid_handler);
	install_keyword("version", &vrrp_version_handler);
	install_keyword("priority", &vrrp_prio_handler);
	install_keyword("advert_int", &vrrp_adv_handler);
	install_keyword("virtual_ipaddress", &vrrp_vip_handler);
	install_keyword("virtual_ipaddress_excluded", &vrrp_evip_handler);
	install_keyword("promote_secondaries", &vrrp_promote_secondaries_handler);
	install_keyword("linkbeat_use_polling", &vrrp_linkbeat_handler);
#ifdef _HAVE_FIB_ROUTING_
	install_keyword("virtual_routes", &vrrp_vroutes_handler);
	install_keyword("virtual_rules", &vrrp_vrules_handler);
#endif
	install_keyword("accept", &vrrp_accept_handler);
	install_keyword("no_accept", &vrrp_no_accept_handler);
	install_keyword("skip_check_adv_addr", &vrrp_skip_check_adv_addr_handler);
	install_keyword("strict_mode", &vrrp_strict_mode_handler);
	install_keyword("preempt", &vrrp_preempt_handler);
	install_keyword("nopreempt", &vrrp_nopreempt_handler);
	install_keyword("preempt_delay", &vrrp_preempt_delay_handler);
	install_keyword("debug", &vrrp_debug_handler);
	install_keyword("notify_backup", &vrrp_notify_backup_handler);
	install_keyword("notify_master", &vrrp_notify_master_handler);
	install_keyword("notify_fault", &vrrp_notify_fault_handler);
	install_keyword("notify_stop", &vrrp_notify_stop_handler);
	install_keyword("notify", &vrrp_notify_handler);
	install_keyword("notify_master_rx_lower_pri", vrrp_notify_master_rx_lower_pri);
	install_keyword("smtp_alert", &vrrp_smtp_handler);
#ifdef _WITH_LVS_
	install_keyword("lvs_sync_daemon_interface", &vrrp_lvs_syncd_handler);
#endif
	install_keyword("garp_master_delay", &vrrp_garp_delay_handler);
	install_keyword("garp_master_refresh", &vrrp_garp_refresh_handler);
	install_keyword("garp_master_repeat", &vrrp_garp_rep_handler);
	install_keyword("garp_master_refresh_repeat", &vrrp_garp_refresh_rep_handler);
	install_keyword("garp_lower_prio_delay", &vrrp_garp_lower_prio_delay_handler);
	install_keyword("garp_lower_prio_repeat", &vrrp_garp_lower_prio_rep_handler);
	install_keyword("lower_prio_no_advert", &vrrp_lower_prio_no_advert_handler);
	install_keyword("higher_prio_send_advert", &vrrp_higher_prio_send_advert_handler);
	install_keyword("kernel_rx_buf_size", &kernel_rx_buf_size_handler);
#if defined _WITH_VRRP_AUTH_
	install_keyword("authentication", NULL);
	install_sublevel();
	install_keyword("auth_type", &vrrp_auth_type_handler);
	install_keyword("auth_pass", &vrrp_auth_pass_handler);
	install_sublevel_end();
#endif
	install_keyword_root("vrrp_script", &vrrp_script_handler, active);
	install_keyword("script", &vrrp_vscript_script_handler);
	install_keyword("interval", &vrrp_vscript_interval_handler);
	install_keyword("timeout", &vrrp_vscript_timeout_handler);
	install_keyword("weight", &vrrp_vscript_weight_handler);
	install_keyword("rise", &vrrp_vscript_rise_handler);
	install_keyword("fall", &vrrp_vscript_fall_handler);
	install_keyword("user", &vrrp_vscript_user_handler);
	install_keyword("init_fail", &vrrp_vscript_init_fail_handler);
	install_sublevel_end_handler(&vrrp_vscript_end_handler);

	 
	install_keyword_root("vrrp_track_file", &vrrp_tfile_handler, active);
	install_keyword("file", &vrrp_tfile_file_handler);
	install_keyword("weight", &vrrp_tfile_weight_handler);
	install_keyword("init_file", &vrrp_tfile_init_handler);
	install_sublevel_end_handler(&vrrp_tfile_end_handler);
}
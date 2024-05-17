nm_setting_vpn_get_data_item (NMSettingVPN *setting, const char *key)
{
	g_return_val_if_fail (NM_IS_SETTING_VPN (setting), NULL);

	return (const char *) g_hash_table_lookup (NM_SETTING_VPN_GET_PRIVATE (setting)->data, key);
}

static void hns_xgmac_get_strings(u32 stringset, u8 *data)
{
	char *buff = (char *)data;
	u32 i;

	if (stringset != ETH_SS_STATS)
		return;

	for (i = 0; i < ARRAY_SIZE(g_xgmac_stats_string); i++) {
		snprintf(buff, ETH_GSTRING_LEN, g_xgmac_stats_string[i].desc);
		buff = buff + ETH_GSTRING_LEN;
	}
}
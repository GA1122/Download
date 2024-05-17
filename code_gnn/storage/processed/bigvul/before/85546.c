static void hns_gmac_get_strings(u32 stringset, u8 *data)
{
	char *buff = (char *)data;
	u32 i;

	if (stringset != ETH_SS_STATS)
		return;

	for (i = 0; i < ARRAY_SIZE(g_gmac_stats_string); i++) {
		snprintf(buff, ETH_GSTRING_LEN, "%s",
			 g_gmac_stats_string[i].desc);
		buff = buff + ETH_GSTRING_LEN;
	}
}
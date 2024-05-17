tBTA_HH_RPT_CACHE_ENTRY * bta_hh_le_co_cache_load (BD_ADDR remote_bda,
                                                   UINT8 *p_num_rpt, UINT8 app_id)
{
    UNUSED(app_id);

 unsigned idx = 0;

 bdstr_t bdstr;
    sprintf(bdstr, "%02x:%02x:%02x:%02x:%02x:%02x",
        remote_bda[0], remote_bda[1], remote_bda[2],
        remote_bda[3], remote_bda[4], remote_bda[5]);

 size_t len = btif_config_get_bin_length(bdstr, "HidReport");
 if (!p_num_rpt && len < sizeof(tBTA_HH_RPT_CACHE_ENTRY))
 return NULL;

 if (len > sizeof(sReportCache))
        len = sizeof(sReportCache);
    btif_config_get_bin(bdstr, "HidReport", (uint8_t *)sReportCache, &len);
 *p_num_rpt = len / sizeof(tBTA_HH_RPT_CACHE_ENTRY);

    BTIF_TRACE_DEBUG("%s() - Loaded %d reports; dev=%s", __FUNCTION__, *p_num_rpt, bdstr);

 return sReportCache;
}

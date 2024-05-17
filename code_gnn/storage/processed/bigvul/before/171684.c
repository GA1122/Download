void bta_hh_le_co_rpt_info(BD_ADDR remote_bda, tBTA_HH_RPT_CACHE_ENTRY *p_entry, UINT8 app_id)
{
    UNUSED(app_id);

 unsigned idx = 0;

 bdstr_t bdstr;
    sprintf(bdstr, "%02x:%02x:%02x:%02x:%02x:%02x",
        remote_bda[0], remote_bda[1], remote_bda[2],
        remote_bda[3], remote_bda[4], remote_bda[5]);

 size_t len = btif_config_get_bin_length(bdstr, "HidReport");
 if (len >= sizeof(tBTA_HH_RPT_CACHE_ENTRY) && len <= sizeof(sReportCache))
 {
        btif_config_get_bin(bdstr, "HidReport", (uint8_t *)sReportCache, &len);
        idx = len / sizeof(tBTA_HH_RPT_CACHE_ENTRY);
 }

 if (idx < BTA_HH_NV_LOAD_MAX)
 {
        memcpy(&sReportCache[idx++], p_entry, sizeof(tBTA_HH_RPT_CACHE_ENTRY));
        btif_config_set_bin(bdstr, "HidReport", (const uint8_t *)sReportCache,
            idx * sizeof(tBTA_HH_RPT_CACHE_ENTRY));
        BTIF_TRACE_DEBUG("%s() - Saving report; dev=%s, idx=%d", __FUNCTION__, bdstr, idx);
 }
}

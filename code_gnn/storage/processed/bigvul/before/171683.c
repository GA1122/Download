void bta_hh_le_co_reset_rpt_cache (BD_ADDR remote_bda, UINT8 app_id)
{
    UNUSED(app_id);

 bdstr_t bdstr;
    sprintf(bdstr, "%02x:%02x:%02x:%02x:%02x:%02x",
        remote_bda[0], remote_bda[1], remote_bda[2],
        remote_bda[3], remote_bda[4], remote_bda[5]);
    btif_config_remove(bdstr, "HidReport");

    BTIF_TRACE_DEBUG("%s() - Reset cache for bda %s", __FUNCTION__, bdstr);
}

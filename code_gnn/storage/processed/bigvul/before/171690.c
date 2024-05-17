BOOLEAN bta_hl_co_get_echo_config(UINT8  app_id,
                                  tBTA_HL_ECHO_CFG *p_echo_cfg)
{
    UINT8               app_idx;
    BOOLEAN             success = FALSE;
 btif_hl_app_cb_t *p_acb;
    tBTA_HL_SUP_FEATURE *p_sup;

    BTIF_TRACE_DEBUG("%s app_id=%d",__FUNCTION__, app_id );

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
        p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
        p_sup = &p_acb->sup_feature;
        p_echo_cfg->max_rx_apdu_size = p_sup->echo_cfg.max_rx_apdu_size;
        p_echo_cfg->max_tx_apdu_size = p_sup->echo_cfg.max_tx_apdu_size;
        success = TRUE;
 }

    BTIF_TRACE_DEBUG("%s success=%d max tx_size=%d rx_size=%d",
                      __FUNCTION__, success, p_echo_cfg->max_tx_apdu_size,
                      p_echo_cfg->max_rx_apdu_size );

 return success;
}

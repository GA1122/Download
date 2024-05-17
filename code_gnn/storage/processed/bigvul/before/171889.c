BOOLEAN btif_hl_load_mdl_config (UINT8 app_id, UINT8 buffer_size,
                                 tBTA_HL_MDL_CFG *p_mdl_buf ){
    UINT8 app_idx;
    BOOLEAN result = FALSE;
 btif_hl_app_cb_t *p_acb;
    tBTA_HL_MDL_CFG *p;
 int i;
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
        p_acb  = BTIF_HL_GET_APP_CB_PTR(app_idx);
 for (i=0, p=p_mdl_buf; i<buffer_size; i++, p++)
 {
            memcpy(p, &p_acb->mdl_cfg[i].base, sizeof(tBTA_HL_MDL_CFG));
 }
        result = TRUE;
 }

    BTIF_TRACE_DEBUG("result=%d", result);
 return result;
}

void btif_hl_release_mcl_sockets(UINT8 app_idx, UINT8 mcl_idx){
    UINT8               i;
 btif_hl_mdl_cb_t *p_dcb;
    BOOLEAN             found= FALSE;
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
 for (i=0; i < BTA_HL_NUM_MDLS_PER_MCL ; i ++)
 {
        p_dcb = BTIF_HL_GET_MDL_CB_PTR(app_idx, mcl_idx, i);
 if (p_dcb && p_dcb->in_use && p_dcb->p_scb)
 {
            BTIF_TRACE_DEBUG("found socket for app_idx=%d mcl_id=%d, mdl_idx=%d", app_idx, mcl_idx, i);
            btif_hl_set_socket_state (p_dcb->p_scb, BTIF_HL_SOC_STATE_W4_REL);
            p_dcb->p_scb = NULL;
            found = TRUE;
 }
 }
 if (found)
        btif_hl_select_close_connected();
}

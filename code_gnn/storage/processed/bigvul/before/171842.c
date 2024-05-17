BOOLEAN btif_hl_create_socket(UINT8 app_idx, UINT8 mcl_idx, UINT8 mdl_idx){
 btif_hl_mcl_cb_t *p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 btif_hl_mdl_cb_t *p_dcb = BTIF_HL_GET_MDL_CB_PTR(app_idx, mcl_idx, mdl_idx);
 btif_hl_soc_cb_t *p_scb = NULL;
    BOOLEAN               status = FALSE;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if (p_dcb && ((p_scb = (btif_hl_soc_cb_t *)GKI_getbuf((UINT16)sizeof(btif_hl_soc_cb_t)))!=NULL))
 {
 if (socketpair(AF_UNIX, SOCK_STREAM, 0, p_scb->socket_id) >= 0)
 {
            BTIF_TRACE_DEBUG("socket id[0]=%d id[1]=%d",p_scb->socket_id[0], p_scb->socket_id[1] );
            p_dcb->p_scb = p_scb;
            p_scb->app_idx = app_idx;
            p_scb->mcl_idx = mcl_idx;
            p_scb->mdl_idx = mdl_idx;
            p_scb->channel_id = p_dcb->channel_id;
            p_scb->mdep_cfg_idx = p_dcb->local_mdep_cfg_idx;
            memcpy(p_scb->bd_addr, p_mcb->bd_addr,sizeof(BD_ADDR));
            btif_hl_set_socket_state(p_scb,  BTIF_HL_SOC_STATE_W4_ADD);
            p_scb->max_s = p_scb->socket_id[1];
            list_append(soc_queue, (void *)p_scb);
            btif_hl_select_wakeup();
            status = TRUE;
 }
 else
 {

            btif_hl_free_buf((void **)&p_scb);
 }
 }

    BTIF_TRACE_DEBUG("%s status=%d", __FUNCTION__, status);
 return status;
}

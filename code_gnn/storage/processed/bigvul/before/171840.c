void btif_hl_close_socket( fd_set *p_org_set){
    BTIF_TRACE_DEBUG("entering %s",__FUNCTION__);
 for (const list_node_t *node = list_begin(soc_queue);
            node != list_end(soc_queue); node = list_next(node)) {
 btif_hl_soc_cb_t *p_scb = list_node(node);
 if (btif_hl_get_socket_state(p_scb) == BTIF_HL_SOC_STATE_W4_REL) {
            BTIF_TRACE_DEBUG("app_idx=%d mcl_id=%d, mdl_idx=%d",
                    p_scb->app_idx, p_scb->mcl_idx, p_scb->mdl_idx);
            btif_hl_set_socket_state(p_scb, BTIF_HL_SOC_STATE_IDLE);
 if (p_scb->socket_id[1] != -1) {
                FD_CLR(p_scb->socket_id[1] , p_org_set);
                shutdown(p_scb->socket_id[1], SHUT_RDWR);
                close(p_scb->socket_id[1]);

 btif_hl_evt_cb_t evt_param;
                evt_param.chan_cb.app_id = (int) btif_hl_get_app_id(p_scb->channel_id);
                memcpy(evt_param.chan_cb.bd_addr, p_scb->bd_addr, sizeof(BD_ADDR));
                evt_param.chan_cb.channel_id = p_scb->channel_id;
                evt_param.chan_cb.fd = p_scb->socket_id[0];
                evt_param.chan_cb.mdep_cfg_index = (int ) p_scb->mdep_cfg_idx;
                evt_param.chan_cb.cb_state = BTIF_HL_CHAN_CB_STATE_DISCONNECTED_PENDING;
 int len = sizeof(btif_hl_send_chan_state_cb_t);
 bt_status_t status = btif_transfer_context (btif_hl_proc_cb_evt,
                        BTIF_HL_SEND_DISCONNECTED_CB,
 (char*) &evt_param, len, NULL);
                ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
 }
 }
 }

 for (const list_node_t *node = list_begin(soc_queue);
        node != list_end(soc_queue); ) {
 btif_hl_soc_cb_t *p_scb = list_node(node);
        node = list_next(node);
 if (btif_hl_get_socket_state(p_scb) == BTIF_HL_SOC_STATE_IDLE) {
 btif_hl_mdl_cb_t *p_dcb = BTIF_HL_GET_MDL_CB_PTR(p_scb->app_idx,
                    p_scb->mcl_idx, p_scb->mdl_idx);
            BTIF_TRACE_DEBUG("idle socket app_idx=%d mcl_id=%d, mdl_idx=%d p_dcb->in_use=%d",
                    p_scb->app_idx, p_scb->mcl_idx, p_scb->mdl_idx, p_dcb->in_use);
            list_remove(soc_queue, p_scb);
            btif_hl_free_buf((void **)&p_scb);
            p_dcb->p_scb = NULL;
 }
        BTIF_TRACE_DEBUG("p_scb=0x%x", p_scb);
 }
    BTIF_TRACE_DEBUG("leaving %s",__FUNCTION__);
}

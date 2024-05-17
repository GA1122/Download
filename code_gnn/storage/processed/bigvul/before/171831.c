void btif_hl_add_socket_to_set(fd_set *p_org_set) {
 btif_hl_mdl_cb_t *p_dcb = NULL;
 btif_hl_mcl_cb_t *p_mcb = NULL;
 btif_hl_app_cb_t *p_acb = NULL;
 btif_hl_evt_cb_t                evt_param;
 bt_status_t                     status;
 int                             len;

    BTIF_TRACE_DEBUG("entering %s",__FUNCTION__);

 for (const list_node_t *node = list_begin(soc_queue);
            node != list_end(soc_queue); node = list_next(node)) {
 btif_hl_soc_cb_t *p_scb = list_node(node);

        BTIF_TRACE_DEBUG("btif_hl_add_socket_to_set first p_scb=0x%x", p_scb);
 if (btif_hl_get_socket_state(p_scb) == BTIF_HL_SOC_STATE_W4_ADD) {
            btif_hl_set_socket_state(p_scb, BTIF_HL_SOC_STATE_W4_READ);
            FD_SET(p_scb->socket_id[1], p_org_set);
            BTIF_TRACE_DEBUG("found and set socket_id=%d is_set=%d",
                    p_scb->socket_id[1], FD_ISSET(p_scb->socket_id[1], p_org_set));
            p_mcb = BTIF_HL_GET_MCL_CB_PTR(p_scb->app_idx, p_scb->mcl_idx);
            p_dcb = BTIF_HL_GET_MDL_CB_PTR(p_scb->app_idx, p_scb->mcl_idx, p_scb->mdl_idx);
            p_acb = BTIF_HL_GET_APP_CB_PTR(p_scb->app_idx);
 if (p_mcb && p_dcb) {
                btif_hl_stop_timer_using_handle(p_mcb->mcl_handle);
                evt_param.chan_cb.app_id = p_acb->app_id;
                memcpy(evt_param.chan_cb.bd_addr, p_mcb->bd_addr, sizeof(BD_ADDR));
                evt_param.chan_cb.channel_id = p_dcb->channel_id;
                evt_param.chan_cb.fd = p_scb->socket_id[0];
                evt_param.chan_cb.mdep_cfg_index = (int ) p_dcb->local_mdep_cfg_idx;
                evt_param.chan_cb.cb_state = BTIF_HL_CHAN_CB_STATE_CONNECTED_PENDING;
                len = sizeof(btif_hl_send_chan_state_cb_t);
                status = btif_transfer_context (btif_hl_proc_cb_evt, BTIF_HL_SEND_CONNECTED_CB,
 (char*) &evt_param, len, NULL);
                ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
 }
 }
 }
    BTIF_TRACE_DEBUG("leaving %s",__FUNCTION__);
}

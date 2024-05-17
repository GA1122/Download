static void btif_hl_upstreams_evt(UINT16 event, char* p_param){
    tBTA_HL *p_data = (tBTA_HL *)p_param;
    UINT8                 app_idx, mcl_idx;
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb = NULL;
 btif_hl_pend_dch_op_t  pending_op;
    BOOLEAN status;

    BTIF_TRACE_DEBUG("%s event %d", __FUNCTION__, event);
    btif_hl_display_calling_process_name();
 switch (event)
 {
 case BTA_HL_REGISTER_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_REGISTER_CFM_EVT");
            BTIF_TRACE_DEBUG("app_id=%d app_handle=%d status=%d ",
                              p_data->reg_cfm.app_id,
                              p_data->reg_cfm.app_handle,
                              p_data->reg_cfm.status );

            btif_hl_proc_reg_cfm(p_data);
 break;
 case BTA_HL_SDP_INFO_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_SDP_INFO_IND_EVT");
            BTIF_TRACE_DEBUG("app_handle=%d ctrl_psm=0x%04x data_psm=0x%04x x_spec=%d mcap_sup_procs=0x%02x",
                              p_data->sdp_info_ind.app_handle,
                              p_data->sdp_info_ind.ctrl_psm,
                              p_data->sdp_info_ind.data_psm,
                              p_data->sdp_info_ind.data_x_spec,
                              p_data->sdp_info_ind.mcap_sup_procs);
 break;

 case BTA_HL_DEREGISTER_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DEREGISTER_CFM_EVT");
            BTIF_TRACE_DEBUG("app_handle=%d status=%d ",
                              p_data->dereg_cfm.app_handle,
                              p_data->dereg_cfm.status );
            btif_hl_proc_dereg_cfm(p_data);
 break;

 case BTA_HL_SDP_QUERY_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_SDP_QUERY_CFM_EVT");
            BTIF_TRACE_DEBUG("app_handle=%d app_id =%d,status =%d",
                              p_data->sdp_query_cfm.app_handle,p_data->sdp_query_cfm.app_id,
                              p_data->sdp_query_cfm.status);

            BTIF_TRACE_DEBUG("DB [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]",
                              p_data->sdp_query_cfm.bd_addr[0], p_data->sdp_query_cfm.bd_addr[1],
                              p_data->sdp_query_cfm.bd_addr[2], p_data->sdp_query_cfm.bd_addr[3],
                              p_data->sdp_query_cfm.bd_addr[4], p_data->sdp_query_cfm.bd_addr[5]);

 if (p_data->sdp_query_cfm.status == BTA_HL_STATUS_OK)
                status = btif_hl_proc_sdp_query_cfm(p_data);
 else
                status = FALSE;

 if (!status)
 {
                BTIF_TRACE_DEBUG("BTA_HL_SDP_QUERY_CFM_EVT Status = %d",
                                                        p_data->sdp_query_cfm.status);
 if (btif_hl_find_app_idx_using_app_id(p_data->sdp_query_cfm.app_id, &app_idx))
 {
                    p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (btif_hl_find_mcl_idx(app_idx, p_data->sdp_query_cfm.bd_addr, &mcl_idx))
 {
                        p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if ( (p_mcb->cch_oper ==  BTIF_HL_CCH_OP_MDEP_FILTERING) ||
 (p_mcb->cch_oper == BTIF_HL_CCH_OP_DCH_OPEN) )
 {
                            pending_op = p_mcb->pcb.op;
 switch (pending_op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
                                    btif_hl_send_setup_disconnected_cb(app_idx, mcl_idx);
 break;
 case BTIF_HL_PEND_DCH_OP_RECONNECT:
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
 default:
 break;
 }
 if (!p_mcb->is_connected)
                                btif_hl_clean_mcl_cb(app_idx, mcl_idx);
 }
 }
 }
 }

 break;


 case BTA_HL_CCH_OPEN_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CCH_OPEN_CFM_EVT");
            BTIF_TRACE_DEBUG("app_id=%d,app_handle=%d mcl_handle=%d status =%d",
                              p_data->cch_open_cfm.app_id,
                              p_data->cch_open_cfm.app_handle,
                              p_data->cch_open_cfm.mcl_handle,
                              p_data->cch_open_cfm.status);
            BTIF_TRACE_DEBUG("DB [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]",
                              p_data->cch_open_cfm.bd_addr[0], p_data->cch_open_cfm.bd_addr[1],
                              p_data->cch_open_cfm.bd_addr[2], p_data->cch_open_cfm.bd_addr[3],
                              p_data->cch_open_cfm.bd_addr[4], p_data->cch_open_cfm.bd_addr[5]);

 if (p_data->cch_open_cfm.status == BTA_HL_STATUS_OK ||
                        p_data->cch_open_cfm.status == BTA_HL_STATUS_DUPLICATE_CCH_OPEN)
 {
                status = btif_hl_proc_cch_open_cfm(p_data);
 }
 else
 {
                status = FALSE;
 }

 if (!status)
 {
 if (btif_hl_find_app_idx_using_app_id(p_data->cch_open_cfm.app_id, &app_idx))
 {
                    p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (btif_hl_find_mcl_idx(app_idx, p_data->cch_open_cfm.bd_addr, &mcl_idx))
 {
                        p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
                        pending_op = p_mcb->pcb.op;
 switch (pending_op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
                                btif_hl_send_setup_disconnected_cb(app_idx, mcl_idx);
 break;
 case BTIF_HL_PEND_DCH_OP_RECONNECT:
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
 default:
 break;
 }
                        btif_hl_clean_mcl_cb(app_idx, mcl_idx);
 }
 }
 }
 break;

 case BTA_HL_DCH_OPEN_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_OPEN_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_handle=0x%x status=%d ",
                              p_data->dch_open_cfm.mcl_handle,
                              p_data->dch_open_cfm.mdl_handle,
                              p_data->dch_open_cfm.status);
            BTIF_TRACE_DEBUG("first_reliable =%d dch_mode=%d local_mdep_id=%d mdl_id=%d mtu=%d",
                              p_data->dch_open_cfm.first_reliable,
                              p_data->dch_open_cfm.dch_mode,
                              p_data->dch_open_cfm.local_mdep_id,
                              p_data->dch_open_cfm.mdl_id,
                              p_data->dch_open_cfm.mtu);
 if (p_data->dch_open_cfm.status == BTA_HL_STATUS_OK)
 {
                status = btif_hl_proc_dch_open_cfm(p_data);
 }
 else
 {
                status = FALSE;
 }

 if (!status)
 {
 if (btif_hl_find_mcl_idx_using_handle(p_data->dch_open_cfm.mcl_handle,&app_idx, &mcl_idx))
 {
                    p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
                    pending_op = p_mcb->pcb.op;
 switch (pending_op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
                            btif_hl_send_setup_disconnected_cb(app_idx, mcl_idx);
 break;
 case BTIF_HL_PEND_DCH_OP_RECONNECT:
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
 default:
 break;
 }
 }
 }
 break;


 case BTA_HL_CCH_OPEN_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CCH_OPEN_IND_EVT");
            BTIF_TRACE_DEBUG("app_handle=%d mcl_handle=%d",
                              p_data->cch_open_ind.app_handle,
                              p_data->cch_open_ind.mcl_handle);
            BTIF_TRACE_DEBUG("DB [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]",
                              p_data->cch_open_ind.bd_addr[0], p_data->cch_open_ind.bd_addr[1],
                              p_data->cch_open_ind.bd_addr[2], p_data->cch_open_ind.bd_addr[3],
                              p_data->cch_open_ind.bd_addr[4], p_data->cch_open_ind.bd_addr[5]);

            btif_hl_proc_cch_open_ind(p_data);
 break;

 case BTA_HL_DCH_CREATE_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_CREATE_IND_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d",
                              p_data->dch_create_ind.mcl_handle );
            BTIF_TRACE_DEBUG("local_mdep_id =%d mdl_id=%d cfg=%d",
                              p_data->dch_create_ind.local_mdep_id,
                              p_data->dch_create_ind.mdl_id,
                              p_data->dch_create_ind.cfg);
            btif_hl_proc_create_ind(p_data);
 break;

 case BTA_HL_DCH_OPEN_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_OPEN_IND_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_handle=0x%x",
                              p_data->dch_open_ind.mcl_handle,
                              p_data->dch_open_ind.mdl_handle );
            BTIF_TRACE_DEBUG("first_reliable =%d dch_mode=%d local_mdep_id=%d mdl_id=%d mtu=%d",
                              p_data->dch_open_ind.first_reliable,
                              p_data->dch_open_ind.dch_mode,
                              p_data->dch_open_ind.local_mdep_id,
                              p_data->dch_open_ind.mdl_id,
                              p_data->dch_open_ind.mtu);

            btif_hl_proc_dch_open_ind(p_data);
 break;

 case BTA_HL_DELETE_MDL_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DELETE_MDL_IND_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_id=0x%x",
                              p_data->delete_mdl_ind.mcl_handle,
                              p_data->delete_mdl_ind.mdl_id);
 break;

 case BTA_HL_DELETE_MDL_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DELETE_MDL_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_id=0x%x status=%d",
                              p_data->delete_mdl_cfm.mcl_handle,
                              p_data->delete_mdl_cfm.mdl_id,
                              p_data->delete_mdl_cfm.status);

 if (btif_hl_find_app_idx_using_deleted_mdl_id( p_data->delete_mdl_cfm.mdl_id,
 &app_idx))
 {
                p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
                btif_hl_send_destroyed_cb(p_acb);
                btif_hl_clean_delete_mdl(&p_acb->delete_mdl);
 }
 break;

 case BTA_HL_DCH_RECONNECT_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_RECONNECT_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_handle=%d status=%d   ",
                              p_data->dch_reconnect_cfm.mcl_handle,
                              p_data->dch_reconnect_cfm.mdl_handle,
                              p_data->dch_reconnect_cfm.status);
            BTIF_TRACE_DEBUG("first_reliable =%d dch_mode=%d mdl_id=%d mtu=%d",
                              p_data->dch_reconnect_cfm.first_reliable,
                              p_data->dch_reconnect_cfm.dch_mode,
                              p_data->dch_reconnect_cfm.mdl_id,
                              p_data->dch_reconnect_cfm.mtu);


 if (p_data->dch_reconnect_cfm.status == BTA_HL_STATUS_OK)
 {
                status = btif_hl_proc_dch_reconnect_cfm(p_data);
 }
 else
 {
                status = FALSE;
 }

 if (!status)
 {
 if (btif_hl_find_mcl_idx_using_handle(p_data->dch_open_cfm.mcl_handle,&app_idx, &mcl_idx))
 {
                    p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
                    pending_op = p_mcb->pcb.op;
 switch (pending_op)
 {
 case BTIF_HL_PEND_DCH_OP_OPEN:
                            btif_hl_send_setup_disconnected_cb(app_idx, mcl_idx);
 break;
 case BTIF_HL_PEND_DCH_OP_RECONNECT:
 case BTIF_HL_PEND_DCH_OP_DELETE_MDL:
 default:
 break;
 }
 }
 }

 break;

 case BTA_HL_CCH_CLOSE_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CCH_CLOSE_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d status =%d",
                              p_data->cch_close_cfm.mcl_handle,
                              p_data->cch_close_cfm.status);
 if (p_data->cch_close_cfm.status == BTA_HL_STATUS_OK)
 {
                btif_hl_proc_cch_close_cfm(p_data);
 }
 break;

 case BTA_HL_CCH_CLOSE_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CCH_CLOSE_IND_EVT");
            BTIF_TRACE_DEBUG("mcl_handle =%d intentional_close=%s",
                              p_data->cch_close_ind.mcl_handle,
 (p_data->cch_close_ind.intentional?"Yes":"No"));

            btif_hl_proc_cch_close_ind(p_data);
 break;

 case BTA_HL_DCH_CLOSE_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_CLOSE_IND_EVT");
            BTIF_TRACE_DEBUG("mdl_handle=%d intentional_close=%s",
                              p_data->dch_close_ind.mdl_handle,
 (p_data->dch_close_ind.intentional?"Yes":"No") );

            btif_hl_proc_dch_close_ind(p_data);
 break;

 case BTA_HL_DCH_CLOSE_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_CLOSE_CFM_EVT");
            BTIF_TRACE_DEBUG("mdl_handle=%d status=%d ",
                              p_data->dch_close_cfm.mdl_handle,
                              p_data->dch_close_cfm.status);

 if (p_data->dch_close_cfm.status == BTA_HL_STATUS_OK)
 {
                btif_hl_proc_dch_close_cfm(p_data);
 }
 break;

 case BTA_HL_DCH_ECHO_TEST_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_ECHO_TEST_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d    status=%d",
                              p_data->echo_test_cfm.mcl_handle,
                              p_data->echo_test_cfm.status );
  
 break;


 case BTA_HL_DCH_RECONNECT_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_RECONNECT_IND_EVT");

            BTIF_TRACE_DEBUG("mcl_handle=%d mdl_handle=5d",
                              p_data->dch_reconnect_ind.mcl_handle,
                              p_data->dch_reconnect_ind.mdl_handle );
            BTIF_TRACE_DEBUG("first_reliable =%d dch_mode=%d mdl_id=%d mtu=%d",
                              p_data->dch_reconnect_ind.first_reliable,
                              p_data->dch_reconnect_ind.dch_mode,
                              p_data->dch_reconnect_ind.mdl_id,
                              p_data->dch_reconnect_ind.mtu);

            btif_hl_proc_dch_reconnect_ind(p_data);
 break;

 case BTA_HL_CONG_CHG_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CONG_CHG_IND_EVT");
            BTIF_TRACE_DEBUG("mdl_handle=%d cong =%d",
                              p_data->dch_cong_ind.mdl_handle,
                              p_data->dch_cong_ind.cong);
            btif_hl_proc_dch_cong_ind(p_data);
 break;

 case BTA_HL_DCH_ABORT_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_ABORT_IND_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d",
                              p_data->dch_abort_ind.mcl_handle );
            btif_hl_proc_abort_ind(p_data->dch_abort_ind.mcl_handle);
 break;
 case BTA_HL_DCH_ABORT_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_ABORT_CFM_EVT");
            BTIF_TRACE_DEBUG("mcl_handle=%d status =%d",
                              p_data->dch_abort_cfm.mcl_handle,
                              p_data->dch_abort_cfm.status);
 if (p_data->dch_abort_cfm.status == BTA_HL_STATUS_OK)
 {
                btif_hl_proc_abort_cfm(p_data->dch_abort_ind.mcl_handle);
 }
 break;

 case BTA_HL_DCH_SEND_DATA_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_SEND_DATA_CFM_EVT");
            BTIF_TRACE_DEBUG("mdl_handle=0x%x status =%d",
                              p_data->dch_send_data_cfm.mdl_handle,
                              p_data->dch_send_data_cfm.status);
            btif_hl_proc_send_data_cfm(p_data->dch_send_data_cfm.mdl_handle,
                                       p_data->dch_send_data_cfm.status);
 break;

 case BTA_HL_DCH_RCV_DATA_IND_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_DCH_RCV_DATA_IND_EVT");
            BTIF_TRACE_DEBUG("mdl_handle=0x%x ",
                              p_data->dch_rcv_data_ind.mdl_handle);
  
 break;

 default:
            BTIF_TRACE_DEBUG("Unknown Event (0x%02x)...", event);
 break;
 }
}

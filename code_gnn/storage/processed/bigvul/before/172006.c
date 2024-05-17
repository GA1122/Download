static bt_status_t send_passthrough_cmd(bt_bdaddr_t *bd_addr, uint8_t key_code, uint8_t key_state)
{
    tAVRC_STS status = BT_STATUS_UNSUPPORTED;
#if (AVRC_CTLR_INCLUDED == TRUE)
    CHECK_RC_CONNECTED
 rc_transaction_t *p_transaction=NULL;
    BTIF_TRACE_DEBUG("%s: key-code: %d, key-state: %d", __FUNCTION__,
                                                    key_code, key_state);
 if (btif_rc_cb.rc_features & BTA_AV_FEAT_RCTG)
 {
 bt_status_t tran_status = get_transaction(&p_transaction);
 if(BT_STATUS_SUCCESS == tran_status && NULL != p_transaction)
 {
            BTA_AvRemoteCmd(btif_rc_cb.rc_handle, p_transaction->lbl,
 (tBTA_AV_RC)key_code, (tBTA_AV_STATE)key_state);
            status =  BT_STATUS_SUCCESS;
            BTIF_TRACE_DEBUG("%s: succesfully sent passthrough command to BTA", __FUNCTION__);
 }
 else
 {
            status =  BT_STATUS_FAIL;
            BTIF_TRACE_DEBUG("%s: error in fetching transaction", __FUNCTION__);
 }
 }
 else
 {
        status =  BT_STATUS_FAIL;
        BTIF_TRACE_DEBUG("%s: feature not supported", __FUNCTION__);
 }
#else
    BTIF_TRACE_DEBUG("%s: feature not enabled", __FUNCTION__);
#endif
 return status;
}

static bt_status_t set_volume(uint8_t volume)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
    CHECK_RC_CONNECTED
    tAVRC_STS status = BT_STATUS_UNSUPPORTED;
 rc_transaction_t *p_transaction=NULL;

 if(btif_rc_cb.rc_volume==volume)
 {
        status=BT_STATUS_DONE;
        BTIF_TRACE_ERROR("%s: volume value already set earlier: 0x%02x",__FUNCTION__, volume);
 return status;
 }

 if ((btif_rc_cb.rc_features & BTA_AV_FEAT_RCTG) &&
 (btif_rc_cb.rc_features & BTA_AV_FEAT_ADV_CTRL))
 {
        tAVRC_COMMAND avrc_cmd = {0};
        BT_HDR *p_msg = NULL;

        BTIF_TRACE_DEBUG("%s: Peer supports absolute volume. newVolume=%d", __FUNCTION__, volume);
        avrc_cmd.volume.opcode = AVRC_OP_VENDOR;
        avrc_cmd.volume.pdu = AVRC_PDU_SET_ABSOLUTE_VOLUME;
        avrc_cmd.volume.status = AVRC_STS_NO_ERROR;
        avrc_cmd.volume.volume = volume;

 if (AVRC_BldCommand(&avrc_cmd, &p_msg) == AVRC_STS_NO_ERROR)
 {
 bt_status_t tran_status=get_transaction(&p_transaction);
 if(BT_STATUS_SUCCESS == tran_status && NULL!=p_transaction)
 {
                BTIF_TRACE_DEBUG("%s msgreq being sent out with label %d",
                                   __FUNCTION__,p_transaction->lbl);
                BTA_AvMetaCmd(btif_rc_cb.rc_handle,p_transaction->lbl, AVRC_CMD_CTRL, p_msg);
                status =  BT_STATUS_SUCCESS;
 }
 else
 {
 if(NULL!=p_msg)
                   GKI_freebuf(p_msg);
                BTIF_TRACE_ERROR("%s: failed to obtain transaction details. status: 0x%02x",
                                    __FUNCTION__, tran_status);
                status = BT_STATUS_FAIL;
 }
 }
 else
 {
            BTIF_TRACE_ERROR("%s: failed to build absolute volume command. status: 0x%02x",
                                __FUNCTION__, status);
            status = BT_STATUS_FAIL;
 }
 }
 else
        status=BT_STATUS_NOT_READY;
 return status;
}

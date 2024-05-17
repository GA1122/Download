static void register_volumechange (UINT8 lbl)
{
    tAVRC_COMMAND avrc_cmd = {0};
    BT_HDR *p_msg = NULL;
    tAVRC_STS BldResp=AVRC_STS_BAD_CMD;
 rc_transaction_t *p_transaction=NULL;

    BTIF_TRACE_DEBUG("%s called with label:%d",__FUNCTION__,lbl);

    avrc_cmd.cmd.opcode=0x00;
    avrc_cmd.pdu = AVRC_PDU_REGISTER_NOTIFICATION;
    avrc_cmd.reg_notif.event_id = AVRC_EVT_VOLUME_CHANGE;
    avrc_cmd.reg_notif.status = AVRC_STS_NO_ERROR;

 BldResp=AVRC_BldCommand(&avrc_cmd, &p_msg);
 if(AVRC_STS_NO_ERROR==BldResp && p_msg)
 {
         p_transaction=get_transaction_by_lbl(lbl);
 if(NULL!=p_transaction)
 {
             BTA_AvMetaCmd(btif_rc_cb.rc_handle,p_transaction->lbl, AVRC_CMD_NOTIF, p_msg);
             BTIF_TRACE_DEBUG("%s:BTA_AvMetaCmd called",__FUNCTION__);
 }
 else
 {
 if(NULL!=p_msg)
               GKI_freebuf(p_msg);
            BTIF_TRACE_ERROR("%s transaction not obtained with label: %d",__FUNCTION__,lbl);
 }
 }
 else
        BTIF_TRACE_ERROR("%s failed to build command:%d",__FUNCTION__,BldResp);
}

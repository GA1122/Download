static void handle_rc_metamsg_rsp(tBTA_AV_META_MSG *pmeta_msg)
{
    tAVRC_RESPONSE    avrc_response = {0};
    UINT8             scratch_buf[512] = {0};
    tAVRC_STS status = BT_STATUS_UNSUPPORTED;

 if(AVRC_OP_VENDOR==pmeta_msg->p_msg->hdr.opcode &&(AVRC_RSP_CHANGED==pmeta_msg->code
 || AVRC_RSP_INTERIM==pmeta_msg->code || AVRC_RSP_ACCEPT==pmeta_msg->code
 || AVRC_RSP_REJ==pmeta_msg->code || AVRC_RSP_NOT_IMPL==pmeta_msg->code))
 {
        status=AVRC_ParsResponse(pmeta_msg->p_msg, &avrc_response, scratch_buf, sizeof(scratch_buf));
        BTIF_TRACE_DEBUG("%s: code %d,event ID %d,PDU %x,parsing status %d, label:%d",
          __FUNCTION__,pmeta_msg->code,avrc_response.reg_notif.event_id,avrc_response.reg_notif.pdu,
          status, pmeta_msg->label);

 if (status != AVRC_STS_NO_ERROR)
 {
 if(AVRC_PDU_REGISTER_NOTIFICATION==avrc_response.rsp.pdu
 && AVRC_EVT_VOLUME_CHANGE==avrc_response.reg_notif.event_id
 && btif_rc_cb.rc_vol_label==pmeta_msg->label)
 {
                btif_rc_cb.rc_vol_label=MAX_LABEL;
                release_transaction(btif_rc_cb.rc_vol_label);
 }
 else if(AVRC_PDU_SET_ABSOLUTE_VOLUME==avrc_response.rsp.pdu)
 {
                release_transaction(pmeta_msg->label);
 }
 return;
 }
 else if(AVRC_PDU_REGISTER_NOTIFICATION==avrc_response.rsp.pdu
 && AVRC_EVT_VOLUME_CHANGE==avrc_response.reg_notif.event_id
 && btif_rc_cb.rc_vol_label!=pmeta_msg->label)
 {
                BTIF_TRACE_DEBUG("%s:Discarding register notfn in rsp.code: %d and label %d",
                __FUNCTION__, pmeta_msg->code, pmeta_msg->label);
 return;
 }
 }
 else
 {
        BTIF_TRACE_DEBUG("%s:Received vendor dependent in adv ctrl rsp. code: %d len: %d. Not processing it.",
        __FUNCTION__, pmeta_msg->code, pmeta_msg->len);
 return;
 }

 if(AVRC_PDU_REGISTER_NOTIFICATION==avrc_response.rsp.pdu
 && AVRC_EVT_VOLUME_CHANGE==avrc_response.reg_notif.event_id
 && AVRC_RSP_CHANGED==pmeta_msg->code)
 {
  
         register_volumechange(btif_rc_cb.rc_vol_label);
 }
 else if(AVRC_PDU_SET_ABSOLUTE_VOLUME==avrc_response.rsp.pdu)
 {
  
          release_transaction(pmeta_msg->label);
 }

     BTIF_TRACE_EVENT("%s: Passing received metamsg response to app. pdu: %s",
             __FUNCTION__, dump_rc_pdu(avrc_response.pdu));
     btif_rc_upstreams_rsp_evt((uint16_t)avrc_response.rsp.pdu, &avrc_response, pmeta_msg->code,
                                pmeta_msg->label);
}

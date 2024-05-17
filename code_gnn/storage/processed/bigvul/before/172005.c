static void send_metamsg_rsp (UINT8 rc_handle, UINT8 label, tBTA_AV_CODE code,
    tAVRC_RESPONSE *pmetamsg_resp)
{
    UINT8 ctype;

 if (!pmetamsg_resp)
 {
        BTIF_TRACE_WARNING("%s: Invalid response received from application", __FUNCTION__);
 return;
 }

    BTIF_TRACE_EVENT("+%s: rc_handle: %d, label: %d, code: 0x%02x, pdu: %s", __FUNCTION__,
        rc_handle, label, code, dump_rc_pdu(pmetamsg_resp->rsp.pdu));

 if (pmetamsg_resp->rsp.status != AVRC_STS_NO_ERROR)
 {
        ctype = AVRC_RSP_REJ;
 }
 else
 {
 if ( code < AVRC_RSP_NOT_IMPL)
 {
 if (code == AVRC_CMD_NOTIF)
 {
               ctype = AVRC_RSP_INTERIM;
 }
 else if (code == AVRC_CMD_STATUS)
 {
               ctype = AVRC_RSP_IMPL_STBL;
 }
 else
 {
               ctype = AVRC_RSP_ACCEPT;
 }
 }
 else
 {
            ctype = code;
 }
 }
  
 if((pmetamsg_resp->rsp.pdu == AVRC_PDU_REGISTER_NOTIFICATION) && (code == AVRC_RSP_CHANGED))
 {
        BOOLEAN bSent = FALSE;
        UINT8   event_id = pmetamsg_resp->reg_notif.event_id;
        BOOLEAN bNotify = (btif_rc_cb.rc_connected) && (btif_rc_cb.rc_notif[event_id-1].bNotify);

  
        btif_rc_cb.rc_notif[event_id-1].bNotify = FALSE;
        BTIF_TRACE_DEBUG("%s rc_handle: %d. event_id: 0x%02d bNotify:%u", __FUNCTION__,
             btif_rc_cb.rc_handle, event_id, bNotify);
 if (bNotify)
 {
            BT_HDR *p_msg = NULL;
            tAVRC_STS status;

 if (AVRC_STS_NO_ERROR == (status = AVRC_BldResponse(btif_rc_cb.rc_handle,
                pmetamsg_resp, &p_msg)) )
 {
                BTIF_TRACE_DEBUG("%s Sending notification to rc_handle: %d. event_id: 0x%02d",
                     __FUNCTION__, btif_rc_cb.rc_handle, event_id);
                bSent = TRUE;
                BTA_AvMetaRsp(btif_rc_cb.rc_handle, btif_rc_cb.rc_notif[event_id-1].label,
                    ctype, p_msg);
 }
 else
 {
                BTIF_TRACE_WARNING("%s failed to build metamsg response. status: 0x%02x",
                    __FUNCTION__, status);
 }

 }

 if (!bSent)
 {
            BTIF_TRACE_DEBUG("%s: Notification not sent, as there are no RC connections or the \
                CT has not subscribed for event_id: %s", __FUNCTION__, dump_rc_notification_event_id(event_id));
 }
 }
 else
 {
  

        BT_HDR *p_msg = NULL;
        tAVRC_STS status;

        status = AVRC_BldResponse(rc_handle, pmetamsg_resp, &p_msg);

 if (status == AVRC_STS_NO_ERROR)
 {
            BTA_AvMetaRsp(rc_handle, label, ctype, p_msg);
 }
 else
 {
            BTIF_TRACE_ERROR("%s: failed to build metamsg response. status: 0x%02x",
                __FUNCTION__, status);
 }
 }
}

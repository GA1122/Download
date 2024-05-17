static void btif_rc_upstreams_rsp_evt(UINT16 event, tAVRC_RESPONSE *pavrc_resp, UINT8 ctype, UINT8 label)
{
    BTIF_TRACE_EVENT("%s pdu: %s handle: 0x%x ctype:%x label:%x", __FUNCTION__,
        dump_rc_pdu(pavrc_resp->pdu), btif_rc_cb.rc_handle, ctype, label);

#if (AVRC_ADV_CTRL_INCLUDED == TRUE)
 switch (event)
 {
 case AVRC_PDU_REGISTER_NOTIFICATION:
 {
 if(AVRC_RSP_CHANGED==ctype)
                 btif_rc_cb.rc_volume=pavrc_resp->reg_notif.param.volume;
             HAL_CBACK(bt_rc_callbacks, volume_change_cb, pavrc_resp->reg_notif.param.volume,ctype)
 }
 break;

 case AVRC_PDU_SET_ABSOLUTE_VOLUME:
 {
            BTIF_TRACE_DEBUG("Set absolute volume change event received: volume %d,ctype %d",
                pavrc_resp->volume.volume,ctype);
 if(AVRC_RSP_ACCEPT==ctype)
                btif_rc_cb.rc_volume=pavrc_resp->volume.volume;
            HAL_CBACK(bt_rc_callbacks,volume_change_cb,pavrc_resp->volume.volume,ctype)
 }
 break;

 default:
 return;
 }
#endif
}

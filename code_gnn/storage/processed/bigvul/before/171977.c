static void btif_rc_upstreams_evt(UINT16 event, tAVRC_COMMAND *pavrc_cmd, UINT8 ctype, UINT8 label)
{
    BTIF_TRACE_EVENT("%s pdu: %s handle: 0x%x ctype:%x label:%x", __FUNCTION__,
        dump_rc_pdu(pavrc_cmd->pdu), btif_rc_cb.rc_handle, ctype, label);

 switch (event)
 {
 case AVRC_PDU_GET_PLAY_STATUS:
 {
            FILL_PDU_QUEUE(IDX_GET_PLAY_STATUS_RSP, ctype, label, TRUE)
            HAL_CBACK(bt_rc_callbacks, get_play_status_cb);
 }
 break;
 case AVRC_PDU_LIST_PLAYER_APP_ATTR:
 case AVRC_PDU_LIST_PLAYER_APP_VALUES:
 case AVRC_PDU_GET_CUR_PLAYER_APP_VALUE:
 case AVRC_PDU_SET_PLAYER_APP_VALUE:
 case AVRC_PDU_GET_PLAYER_APP_ATTR_TEXT:
 case AVRC_PDU_GET_PLAYER_APP_VALUE_TEXT:
 {
  
            send_reject_response (btif_rc_cb.rc_handle, label, pavrc_cmd->pdu, AVRC_STS_BAD_CMD);
 }
 break;
 case AVRC_PDU_GET_ELEMENT_ATTR:
 {
 btrc_media_attr_t element_attrs[BTRC_MAX_ELEM_ATTR_SIZE];
            UINT8 num_attr;
             memset(&element_attrs, 0, sizeof(element_attrs));
 if (pavrc_cmd->get_elem_attrs.num_attr == 0)
 {
  
 int attr_cnt;
                num_attr = BTRC_MAX_ELEM_ATTR_SIZE;
 for (attr_cnt = 0; attr_cnt < BTRC_MAX_ELEM_ATTR_SIZE; attr_cnt++)
 {
                    element_attrs[attr_cnt] = attr_cnt + 1;
 }
 }
 else if (pavrc_cmd->get_elem_attrs.num_attr == 0xFF)
 {
  
                send_reject_response (btif_rc_cb.rc_handle, label, pavrc_cmd->pdu,
                    AVRC_STS_BAD_PARAM);
 return;
 }
 else
 {
 int attr_cnt, filled_attr_count;

                num_attr = 0;
  
 for (attr_cnt = 0; (attr_cnt < pavrc_cmd->get_elem_attrs.num_attr) &&
 (num_attr < AVRC_MAX_NUM_MEDIA_ATTR_ID); attr_cnt++)
 {
 if ((pavrc_cmd->get_elem_attrs.attrs[attr_cnt] > 0) &&
 (pavrc_cmd->get_elem_attrs.attrs[attr_cnt] <= AVRC_MAX_NUM_MEDIA_ATTR_ID))
 {
  
 for (filled_attr_count = 0; filled_attr_count < num_attr; filled_attr_count++)
 {
 if (element_attrs[filled_attr_count] == pavrc_cmd->get_elem_attrs.attrs[attr_cnt])
 break;
 }
 if (filled_attr_count == num_attr)
 {
                            element_attrs[num_attr] = pavrc_cmd->get_elem_attrs.attrs[attr_cnt];
                            num_attr++;
 }
 }
 }
 }
            FILL_PDU_QUEUE(IDX_GET_ELEMENT_ATTR_RSP, ctype, label, TRUE);
            HAL_CBACK(bt_rc_callbacks, get_element_attr_cb, num_attr, element_attrs);
 }
 break;
 case AVRC_PDU_REGISTER_NOTIFICATION:
 {
 if(pavrc_cmd->reg_notif.event_id == BTRC_EVT_PLAY_POS_CHANGED &&
                pavrc_cmd->reg_notif.param == 0)
 {
                BTIF_TRACE_WARNING("%s Device registering position changed with illegal param 0.",
                    __FUNCTION__);
                send_reject_response (btif_rc_cb.rc_handle, label, pavrc_cmd->pdu, AVRC_STS_BAD_PARAM);
  
                btif_rc_cb.rc_notif[BTRC_EVT_PLAY_POS_CHANGED - 1].bNotify = FALSE;
 return;
 }
            HAL_CBACK(bt_rc_callbacks, register_notification_cb, pavrc_cmd->reg_notif.event_id,
                pavrc_cmd->reg_notif.param);
 }
 break;
 case AVRC_PDU_INFORM_DISPLAY_CHARSET:
 {
            tAVRC_RESPONSE avrc_rsp;
            BTIF_TRACE_EVENT("%s() AVRC_PDU_INFORM_DISPLAY_CHARSET", __FUNCTION__);
 if(btif_rc_cb.rc_connected == TRUE)
 {
                memset(&(avrc_rsp.inform_charset), 0, sizeof(tAVRC_RSP));
                avrc_rsp.inform_charset.opcode=opcode_from_pdu(AVRC_PDU_INFORM_DISPLAY_CHARSET);
                avrc_rsp.inform_charset.pdu=AVRC_PDU_INFORM_DISPLAY_CHARSET;
                avrc_rsp.inform_charset.status=AVRC_STS_NO_ERROR;
                send_metamsg_rsp(btif_rc_cb.rc_handle, label, ctype, &avrc_rsp);
 }
 }
 break;
 default:
 {
        send_reject_response (btif_rc_cb.rc_handle, label, pavrc_cmd->pdu,
 (pavrc_cmd->pdu == AVRC_PDU_SEARCH)?AVRC_STS_SEARCH_NOT_SUP:AVRC_STS_BAD_CMD);
 return;
 }
 break;
 }

}

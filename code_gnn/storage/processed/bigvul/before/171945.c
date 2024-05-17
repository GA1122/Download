static void btif_in_pan_generic_evt(UINT16 event, char *p_param)
{
    BTIF_TRACE_EVENT("%s: event=%d", __FUNCTION__, event);
 switch (event) {
 case BTIF_PAN_CB_DISCONNECTING:
 {
 bt_bdaddr_t *bd_addr = (bt_bdaddr_t*)p_param;
 btpan_conn_t* conn = btpan_find_conn_addr(bd_addr->address);
 int btpan_conn_local_role;
 int btpan_remote_role;
            asrt(conn != NULL);
 if (conn) {
                btpan_conn_local_role = bta_role_to_btpan(conn->local_role);
                btpan_remote_role = bta_role_to_btpan(conn->remote_role);
                callback.connection_state_cb(BTPAN_STATE_DISCONNECTING, BT_STATUS_SUCCESS,
 (const bt_bdaddr_t*)conn->peer, btpan_conn_local_role, btpan_remote_role);
 }
 } break;
 default:
 {
            BTIF_TRACE_WARNING("%s : Unknown event 0x%x", __FUNCTION__, event);
 }
 break;
 }
}

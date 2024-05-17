static bt_status_t btpan_disconnect(const bt_bdaddr_t *bd_addr)
{
 btpan_conn_t* conn = btpan_find_conn_addr(bd_addr->address);
 if (conn && conn->handle >= 0)
 {
  
        btif_transfer_context(btif_in_pan_generic_evt, BTIF_PAN_CB_DISCONNECTING,
 (char *)bd_addr, sizeof(bt_bdaddr_t), NULL);
        BTA_PanClose(conn->handle);
 return BT_STATUS_SUCCESS;
 }
 return BT_STATUS_FAIL;
}

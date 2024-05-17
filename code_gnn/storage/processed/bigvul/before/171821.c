static bt_status_t disconnect( bt_bdaddr_t *bd_addr )
{
    CHECK_BTHH_INIT();
 btif_hh_device_t *p_dev;

 if (btif_hh_cb.status == BTIF_HH_DISABLED)
 {
        BTIF_TRACE_WARNING("%s: Error, HH status = %d", __FUNCTION__, btif_hh_cb.status);
 return BT_STATUS_FAIL;
 }
    p_dev = btif_hh_find_connected_dev_by_bda(bd_addr);
 if (p_dev != NULL)
 {
 return btif_transfer_context(btif_hh_handle_evt, BTIF_HH_DISCONNECT_REQ_EVT,
 (char*)bd_addr, sizeof(bt_bdaddr_t), NULL);
 }
 else
 {
        BTIF_TRACE_WARNING("%s: Error, device  not opened.", __FUNCTION__);
 return BT_STATUS_FAIL;
 }
}

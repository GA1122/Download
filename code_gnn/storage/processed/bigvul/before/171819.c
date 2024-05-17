static bt_status_t connect( bt_bdaddr_t *bd_addr)
{
 if(btif_hh_cb.status != BTIF_HH_DEV_CONNECTING)
 {
        btif_transfer_context(btif_hh_handle_evt, BTIF_HH_CONNECT_REQ_EVT,
 (char*)bd_addr, sizeof(bt_bdaddr_t), NULL);
 return BT_STATUS_SUCCESS;
 }
 else
 return BT_STATUS_BUSY;
}

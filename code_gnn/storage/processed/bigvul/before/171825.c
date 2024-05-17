static bt_status_t init( bthh_callbacks_t* callbacks )
{
    UINT32 i;
    BTIF_TRACE_EVENT("%s", __FUNCTION__);

    bt_hh_callbacks = callbacks;
    memset(&btif_hh_cb, 0, sizeof(btif_hh_cb));
 for (i = 0; i < BTIF_HH_MAX_HID; i++){
        btif_hh_cb.devices[i].dev_status = BTHH_CONN_STATE_UNKNOWN;
 }
  
    btif_enable_service(BTA_HID_SERVICE_ID);
 return BT_STATUS_SUCCESS;
}

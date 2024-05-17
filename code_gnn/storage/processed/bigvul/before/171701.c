bt_status_t btif_disable_bluetooth(void)
{
    BTIF_TRACE_DEBUG("BTIF DISABLE BLUETOOTH");

    btif_dm_on_disable();
  
    btif_sock_cleanup();
    btif_pan_cleanup();
    BTA_DisableBluetooth();

 return BT_STATUS_SUCCESS;
}

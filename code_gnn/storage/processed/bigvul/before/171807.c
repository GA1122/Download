btif_hh_device_t *btif_hh_find_connected_dev_by_handle(UINT8 handle)
{
    UINT32 i;
 for (i = 0; i < BTIF_HH_MAX_HID; i++) {
 if (btif_hh_cb.devices[i].dev_status == BTHH_CONN_STATE_CONNECTED &&
            btif_hh_cb.devices[i].dev_handle == handle)
 {
 return &btif_hh_cb.devices[i];
 }
 }
 return NULL;
}

BOOLEAN btif_hh_add_added_dev(bt_bdaddr_t bda, tBTA_HH_ATTR_MASK attr_mask)
{
 int i;
 for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
 if (memcmp(&(btif_hh_cb.added_devices[i].bd_addr), &bda, BD_ADDR_LEN) == 0) {
            BTIF_TRACE_WARNING(" Device %02X:%02X:%02X:%02X:%02X:%02X already added",
                  bda.address[0], bda.address[1], bda.address[2], bda.address[3], bda.address[4], bda.address[5]);
 return FALSE;
 }
 }
 for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
 if (btif_hh_cb.added_devices[i].bd_addr.address[0] == 0 &&
            btif_hh_cb.added_devices[i].bd_addr.address[1] == 0 &&
            btif_hh_cb.added_devices[i].bd_addr.address[2] == 0 &&
            btif_hh_cb.added_devices[i].bd_addr.address[3] == 0 &&
            btif_hh_cb.added_devices[i].bd_addr.address[4] == 0 &&
            btif_hh_cb.added_devices[i].bd_addr.address[5] == 0)
 {
            BTIF_TRACE_WARNING(" Added device %02X:%02X:%02X:%02X:%02X:%02X",
                  bda.address[0], bda.address[1], bda.address[2], bda.address[3], bda.address[4], bda.address[5]);
            memcpy(&(btif_hh_cb.added_devices[i].bd_addr), &bda, BD_ADDR_LEN);
            btif_hh_cb.added_devices[i].dev_handle = BTA_HH_INVALID_HANDLE;
            btif_hh_cb.added_devices[i].attr_mask  = attr_mask;
 return TRUE;
 }
 }

    BTIF_TRACE_WARNING("%s: Error, out of space to add device",__FUNCTION__);
 return FALSE;
}

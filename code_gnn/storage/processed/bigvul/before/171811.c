void btif_hh_remove_device(bt_bdaddr_t bd_addr)
{
 int                    i;
 btif_hh_device_t *p_dev;
 btif_hh_added_device_t *p_added_dev;

    LOG_INFO("%s: bda = %02x:%02x:%02x:%02x:%02x:%02x", __FUNCTION__,
         bd_addr.address[0], bd_addr.address[1], bd_addr.address[2], bd_addr.address[3], bd_addr.address[4], bd_addr.address[5]);

 for (i = 0; i < BTIF_HH_MAX_ADDED_DEV; i++) {
        p_added_dev = &btif_hh_cb.added_devices[i];
 if (memcmp(&(p_added_dev->bd_addr),&bd_addr, 6) == 0) {
            BTA_HhRemoveDev(p_added_dev->dev_handle);
            btif_storage_remove_hid_info(&(p_added_dev->bd_addr));
            memset(&(p_added_dev->bd_addr), 0, 6);
            p_added_dev->dev_handle = BTA_HH_INVALID_HANDLE;
 break;
 }
 }

    p_dev = btif_hh_find_dev_by_bda(&bd_addr);
 if (p_dev == NULL) {
        BTIF_TRACE_WARNING(" Oops, can't find device [%02x:%02x:%02x:%02x:%02x:%02x]",
             bd_addr.address[0], bd_addr.address[1], bd_addr.address[2], bd_addr.address[3], bd_addr.address[4], bd_addr.address[5]);
 return;
 }

  
    HAL_CBACK(bt_hh_callbacks, connection_state_cb, &(p_dev->bd_addr), BTHH_CONN_STATE_DISCONNECTED);

    p_dev->dev_status = BTHH_CONN_STATE_UNKNOWN;
    p_dev->dev_handle = BTA_HH_INVALID_HANDLE;
    p_dev->ready_for_data = FALSE;

 if (btif_hh_cb.device_num > 0) {
        btif_hh_cb.device_num--;
 }
 else {
        BTIF_TRACE_WARNING("%s: device_num = 0", __FUNCTION__);
 }

    p_dev->hh_keep_polling = 0;
    p_dev->hh_poll_thread_id = -1;
    BTIF_TRACE_DEBUG("%s: uhid fd = %d", __FUNCTION__, p_dev->fd);
 if (p_dev->fd >= 0) {
        bta_hh_co_destroy(p_dev->fd);
        p_dev->fd = -1;
 }
}

void bta_hh_co_close(UINT8 dev_handle, UINT8 app_id)
{
    UINT32 i;
 btif_hh_device_t *p_dev = NULL;

    APPL_TRACE_WARNING("%s: dev_handle = %d, app_id = %d", __FUNCTION__, dev_handle, app_id);
 if (dev_handle == BTA_HH_INVALID_HANDLE) {
        APPL_TRACE_WARNING("%s: Oops, dev_handle (%d) is invalid...", __FUNCTION__, dev_handle);
 return;
 }

 for (i = 0; i < BTIF_HH_MAX_HID; i++) {
        p_dev = &btif_hh_cb.devices[i];
 if (p_dev->dev_status != BTHH_CONN_STATE_UNKNOWN && p_dev->dev_handle == dev_handle) {
            APPL_TRACE_WARNING("%s: Found an existing device with the same handle "
 "dev_status = %d, dev_handle =%d"
 ,__FUNCTION__,p_dev->dev_status
 ,p_dev->dev_handle);
            btif_hh_close_poll_thread(p_dev);
 break;
 }
 }
}

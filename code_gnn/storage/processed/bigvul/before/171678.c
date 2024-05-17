void bta_hh_co_data(UINT8 dev_handle, UINT8 *p_rpt, UINT16 len, tBTA_HH_PROTO_MODE mode,
                    UINT8 sub_class, UINT8 ctry_code, BD_ADDR peer_addr, UINT8 app_id)
{
 btif_hh_device_t *p_dev;
    UNUSED(peer_addr);

    APPL_TRACE_DEBUG("%s: dev_handle = %d, subclass = 0x%02X, mode = %d, "
 "ctry_code = %d, app_id = %d",
         __FUNCTION__, dev_handle, sub_class, mode, ctry_code, app_id);

    p_dev = btif_hh_find_connected_dev_by_handle(dev_handle);
 if (p_dev == NULL) {
        APPL_TRACE_WARNING("%s: Error: unknown HID device handle %d", __FUNCTION__, dev_handle);
 return;
 }

 if ((p_dev->fd >= 0) && p_dev->ready_for_data) {
        bta_hh_co_write(p_dev->fd, p_rpt, len);
 }else {
        APPL_TRACE_WARNING("%s: Error: fd = %d, ready %d, len = %d", __FUNCTION__, p_dev->fd, 
                            p_dev->ready_for_data, len);
 }
}

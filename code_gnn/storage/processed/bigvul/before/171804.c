void btif_hh_disconnect(bt_bdaddr_t *bd_addr)
{
 btif_hh_device_t *p_dev;
    p_dev = btif_hh_find_connected_dev_by_bda(bd_addr);
 if (p_dev != NULL)
 {
        BTA_HhClose(p_dev->dev_handle);
 }
 else
        BTIF_TRACE_DEBUG("%s-- Error: device not connected:",__FUNCTION__);
}

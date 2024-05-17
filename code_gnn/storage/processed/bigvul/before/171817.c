bt_status_t btif_hh_virtual_unplug(bt_bdaddr_t *bd_addr)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
 btif_hh_device_t *p_dev;
 char bd_str[18];
    sprintf(bd_str, "%02X:%02X:%02X:%02X:%02X:%02X",
            bd_addr->address[0],  bd_addr->address[1],  bd_addr->address[2],  bd_addr->address[3],
            bd_addr->address[4], bd_addr->address[5]);
    p_dev = btif_hh_find_dev_by_bda(bd_addr);
 if ((p_dev != NULL) && (p_dev->dev_status == BTHH_CONN_STATE_CONNECTED)
 && (p_dev->attr_mask & HID_VIRTUAL_CABLE))
 {
        BTIF_TRACE_DEBUG("%s Sending BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG", __FUNCTION__);
  
        btif_hh_start_vup_timer(bd_addr);
        p_dev->local_vup = TRUE;
        BTA_HhSendCtrl(p_dev->dev_handle, BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG);
 return BT_STATUS_SUCCESS;
 }
 else
 {
        BTIF_TRACE_ERROR("%s: Error, device %s not opened.", __FUNCTION__, bd_str);
 return BT_STATUS_FAIL;
 }
}

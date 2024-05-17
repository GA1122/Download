static bt_status_t set_protocol (bt_bdaddr_t *bd_addr, bthh_protocol_mode_t protocolMode)
{
    CHECK_BTHH_INIT();
 btif_hh_device_t *p_dev;
    UINT8 proto_mode = protocolMode;
    BD_ADDR* bda = (BD_ADDR*) bd_addr;

    BTIF_TRACE_DEBUG("%s:proto_mode = %d", __FUNCTION__,protocolMode);

    BTIF_TRACE_DEBUG("addr = %02X:%02X:%02X:%02X:%02X:%02X",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);

 if (btif_hh_cb.status == BTIF_HH_DISABLED) {
        BTIF_TRACE_ERROR("%s: Error, HH status = %d", __FUNCTION__, btif_hh_cb.status);
 return BT_STATUS_FAIL;
 }

    p_dev = btif_hh_find_connected_dev_by_bda(bd_addr);
 if (p_dev == NULL) {
        BTIF_TRACE_WARNING(" Error, device %02X:%02X:%02X:%02X:%02X:%02X not opened.",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);
 return BT_STATUS_FAIL;
 }
 else if (protocolMode != BTA_HH_PROTO_RPT_MODE && protocolMode != BTA_HH_PROTO_BOOT_MODE) {
        BTIF_TRACE_WARNING("s: Error, device proto_mode = %d.", __FUNCTION__, proto_mode);
 return BT_STATUS_FAIL;
 }
 else {
        BTA_HhSetProtoMode(p_dev->dev_handle, protocolMode);
 }


 return BT_STATUS_SUCCESS;
}

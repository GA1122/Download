static bt_status_t set_report (bt_bdaddr_t *bd_addr, bthh_report_type_t reportType, char* report)
{
    CHECK_BTHH_INIT();
 btif_hh_device_t *p_dev;
    BD_ADDR* bda = (BD_ADDR*) bd_addr;

    BTIF_TRACE_DEBUG("%s:reportType = %d", __FUNCTION__,reportType);

    BTIF_TRACE_DEBUG("addr = %02X:%02X:%02X:%02X:%02X:%02X",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);


 if (btif_hh_cb.status == BTIF_HH_DISABLED) {
        BTIF_TRACE_ERROR("%s: Error, HH status = %d", __FUNCTION__, btif_hh_cb.status);
 return BT_STATUS_FAIL;
 }

    p_dev = btif_hh_find_connected_dev_by_bda(bd_addr);
 if (p_dev == NULL) {
        BTIF_TRACE_ERROR("%s: Error, device %02X:%02X:%02X:%02X:%02X:%02X not opened.",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);
 return BT_STATUS_FAIL;
 }
 else if ( ( (int) reportType) <= BTA_HH_RPTT_RESRV || ( (int) reportType) > BTA_HH_RPTT_FEATURE) {
        BTIF_TRACE_ERROR(" Error, device %02X:%02X:%02X:%02X:%02X:%02X not opened.",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);
 return BT_STATUS_FAIL;
 }
 else {
 int    hex_bytes_filled;
        UINT8  *hexbuf;
        UINT16 len = (strlen(report) + 1) / 2;

        hexbuf = GKI_getbuf(len);
 if (hexbuf == NULL) {
            BTIF_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, len = %d",
                __FUNCTION__, len);
 return BT_STATUS_FAIL;
 }

  
        memset(hexbuf, 0, len);
        hex_bytes_filled = ascii_2_hex(report, len, hexbuf);
        LOG_INFO("Hex bytes filled, hex value: %d", hex_bytes_filled);
 if (hex_bytes_filled) {
            BT_HDR* p_buf = create_pbuf(hex_bytes_filled, hexbuf);
 if (p_buf == NULL) {
                BTIF_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, len = %d",
                                  __FUNCTION__, hex_bytes_filled);
                GKI_freebuf(hexbuf);
 return BT_STATUS_FAIL;
 }
            BTA_HhSetReport(p_dev->dev_handle, reportType, p_buf);
            GKI_freebuf(hexbuf);
 return BT_STATUS_SUCCESS;
 }
        GKI_freebuf(hexbuf);
 return BT_STATUS_FAIL;
 }
}

static bt_status_t get_report (bt_bdaddr_t *bd_addr, bthh_report_type_t reportType, uint8_t reportId, int bufferSize)
{
    CHECK_BTHH_INIT();
 btif_hh_device_t *p_dev;
    BD_ADDR* bda = (BD_ADDR*) bd_addr;

    BTIF_TRACE_DEBUG("%s:proto_mode = %dr_type = %d, rpt_id = %d, buf_size = %d", __FUNCTION__,
          reportType, reportId, bufferSize);

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
 else if ( ((int) reportType) <= BTA_HH_RPTT_RESRV || ((int) reportType) > BTA_HH_RPTT_FEATURE) {
        BTIF_TRACE_ERROR(" Error, device %02X:%02X:%02X:%02X:%02X:%02X not opened.",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);
 return BT_STATUS_FAIL;
 }
 else {
        BTA_HhGetReport(p_dev->dev_handle, reportType,
                        reportId, bufferSize);
 }

 return BT_STATUS_SUCCESS;
}
void btif_hh_setreport(btif_hh_device_t *p_dev, bthh_report_type_t r_type, UINT16 size,
                            UINT8* report)
{
    BT_HDR* p_buf = create_pbuf(size, report);
 if (p_buf == NULL) {
        APPL_TRACE_ERROR("%s: Error, failed to allocate RPT buffer, size = %d", __FUNCTION__, size);
 return;
 }
    BTA_HhSetReport(p_dev->dev_handle, r_type, p_buf);
}

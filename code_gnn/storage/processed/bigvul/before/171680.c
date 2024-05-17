void bta_hh_co_send_hid_info(btif_hh_device_t *p_dev, char *dev_name, UINT16 vendor_id,
                             UINT16 product_id, UINT16 version, UINT8 ctry_code,
 int dscp_len, UINT8 *p_dscp)
{
 int result;
 struct uhid_event ev;

 if (p_dev->fd < 0) {
        APPL_TRACE_WARNING("%s: Error: fd = %d, dscp_len = %d", __FUNCTION__, p_dev->fd, dscp_len);
 return;
 }

    APPL_TRACE_WARNING("%s: fd = %d, name = [%s], dscp_len = %d", __FUNCTION__,
                                                                    p_dev->fd, dev_name, dscp_len);
    APPL_TRACE_WARNING("%s: vendor_id = 0x%04x, product_id = 0x%04x, version= 0x%04x,"
 "ctry_code=0x%02x",__FUNCTION__,
                                                                    vendor_id, product_id,
                                                                    version, ctry_code);

    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_CREATE;
    strncpy((char*)ev.u.create.name, dev_name, sizeof(ev.u.create.name) - 1);
    snprintf((char*)ev.u.create.uniq, sizeof(ev.u.create.uniq),
 "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
             p_dev->bd_addr.address[5], p_dev->bd_addr.address[4],
             p_dev->bd_addr.address[3], p_dev->bd_addr.address[2],
             p_dev->bd_addr.address[1], p_dev->bd_addr.address[0]);
    ev.u.create.rd_size = dscp_len;
    ev.u.create.rd_data = p_dscp;
    ev.u.create.bus = BUS_BLUETOOTH;
    ev.u.create.vendor = vendor_id;
    ev.u.create.product = product_id;
    ev.u.create.version = version;
    ev.u.create.country = ctry_code;
    result = uhid_write(p_dev->fd, &ev);

    APPL_TRACE_WARNING("%s: wrote descriptor to fd = %d, dscp_len = %d, result = %d", __FUNCTION__,
                                                                    p_dev->fd, dscp_len, result);

 if (result) {
        APPL_TRACE_WARNING("%s: Error: failed to send DSCP, result = %d", __FUNCTION__, result);

  
        close(p_dev->fd);
        p_dev->fd = -1;
 }
}

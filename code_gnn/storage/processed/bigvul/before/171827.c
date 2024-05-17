static bt_status_t set_info (bt_bdaddr_t *bd_addr, bthh_hid_info_t hid_info )
{
    CHECK_BTHH_INIT();
    tBTA_HH_DEV_DSCP_INFO dscp_info;
    BD_ADDR* bda = (BD_ADDR*) bd_addr;

    BTIF_TRACE_DEBUG("addr = %02X:%02X:%02X:%02X:%02X:%02X",
 (*bda)[0], (*bda)[1], (*bda)[2], (*bda)[3], (*bda)[4], (*bda)[5]);
    BTIF_TRACE_DEBUG("%s: sub_class = 0x%02x, app_id = %d, vendor_id = 0x%04x, "
 "product_id = 0x%04x, version= 0x%04x",
         __FUNCTION__, hid_info.sub_class,
         hid_info.app_id, hid_info.vendor_id, hid_info.product_id,
         hid_info.version);

 if (btif_hh_cb.status == BTIF_HH_DISABLED)
 {
        BTIF_TRACE_ERROR("%s: Error, HH status = %d", __FUNCTION__, btif_hh_cb.status);
 return BT_STATUS_FAIL;
 }

    dscp_info.vendor_id  = hid_info.vendor_id;
    dscp_info.product_id = hid_info.product_id;
    dscp_info.version    = hid_info.version;
    dscp_info.ctry_code  = hid_info.ctry_code;

    dscp_info.descriptor.dl_len = hid_info.dl_len;
    dscp_info.descriptor.dsc_list = (UINT8 *) GKI_getbuf(dscp_info.descriptor.dl_len);
 if (dscp_info.descriptor.dsc_list == NULL)
 {
        LOG_ERROR("%s: Failed to allocate DSCP for CB", __FUNCTION__);
 return BT_STATUS_FAIL;
 }
    memcpy(dscp_info.descriptor.dsc_list, &(hid_info.dsc_list), hid_info.dl_len);

 if (btif_hh_add_added_dev(*bd_addr, hid_info.attr_mask))
 {
        BTA_HhAddDev(*bda, hid_info.attr_mask, hid_info.sub_class,
                     hid_info.app_id, dscp_info);
 }

    GKI_freebuf(dscp_info.descriptor.dsc_list);

 return BT_STATUS_SUCCESS;
}

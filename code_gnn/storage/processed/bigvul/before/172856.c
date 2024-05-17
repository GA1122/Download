bt_status_t btif_storage_add_hid_device_info(bt_bdaddr_t *remote_bd_addr,
                                                    UINT16 attr_mask, UINT8 sub_class,
                                                    UINT8 app_id, UINT16 vendor_id,
                                                    UINT16 product_id, UINT16 version,
                                                    UINT8 ctry_code, UINT16 ssr_max_latency,
                                                    UINT16 ssr_min_tout, UINT16 dl_len, UINT8 *dsc_list)
{
 bdstr_t bdstr;
    BTIF_TRACE_DEBUG("btif_storage_add_hid_device_info:");
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    btif_config_set_int(bdstr, "HidAttrMask", attr_mask);
    btif_config_set_int(bdstr, "HidSubClass", sub_class);
    btif_config_set_int(bdstr, "HidAppId", app_id);
    btif_config_set_int(bdstr, "HidVendorId", vendor_id);
    btif_config_set_int(bdstr, "HidProductId", product_id);
    btif_config_set_int(bdstr, "HidVersion", version);
    btif_config_set_int(bdstr, "HidCountryCode", ctry_code);
    btif_config_set_int(bdstr, "HidSSRMaxLatency", ssr_max_latency);
    btif_config_set_int(bdstr, "HidSSRMinTimeout", ssr_min_tout);
 if(dl_len > 0)
        btif_config_set_bin(bdstr, "HidDescriptor", dsc_list, dl_len);
    btif_config_save();
 return BT_STATUS_SUCCESS;
}

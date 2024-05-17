bt_status_t btif_storage_load_bonded_hid_info(void)
{
 bt_bdaddr_t bd_addr;
    tBTA_HH_DEV_DSCP_INFO dscp_info;
 uint16_t attr_mask;
 uint8_t  sub_class;
 uint8_t  app_id;

    memset(&dscp_info, 0, sizeof(dscp_info));
 for (const btif_config_section_iter_t *iter = btif_config_section_begin(); iter != btif_config_section_end(); iter = btif_config_section_next(iter)) {
 const char *name = btif_config_section_name(iter);
 if (!string_is_bdaddr(name))
 continue;

        BTIF_TRACE_DEBUG("Remote device:%s", name);
 int value;
 if(btif_in_fetch_bonded_device(name) == BT_STATUS_SUCCESS)
 {
 if(btif_config_get_int(name, "HidAttrMask", &value))
 {
                attr_mask = (uint16_t)value;

                btif_config_get_int(name, "HidSubClass", &value);
                sub_class = (uint8_t)value;

                btif_config_get_int(name, "HidAppId", &value);
                app_id = (uint8_t)value;

                btif_config_get_int(name, "HidVendorId", &value);
                dscp_info.vendor_id = (uint16_t) value;

                btif_config_get_int(name, "HidProductId", &value);
                dscp_info.product_id = (uint16_t) value;

                btif_config_get_int(name, "HidVersion", &value);
                dscp_info.version = (uint8_t) value;

                btif_config_get_int(name, "HidCountryCode", &value);
                dscp_info.ctry_code = (uint8_t) value;

                value = 0;
                btif_config_get_int(name, "HidSSRMaxLatency", &value);
                dscp_info.ssr_max_latency = (uint16_t) value;

                value = 0;
                btif_config_get_int(name, "HidSSRMinTimeout", &value);
                dscp_info.ssr_min_tout = (uint16_t) value;

 size_t len = btif_config_get_bin_length(name, "HidDescriptor");
 if(len > 0)
 {
                    dscp_info.descriptor.dl_len = (uint16_t)len;
                    dscp_info.descriptor.dsc_list = (uint8_t*)alloca(len);
                    btif_config_get_bin(name, "HidDescriptor", (uint8_t *)dscp_info.descriptor.dsc_list, &len);
 }
                string_to_bdaddr(name, &bd_addr);
 if (btif_hh_add_added_dev(bd_addr,attr_mask))
 {
                    BTA_HhAddDev(bd_addr.address, attr_mask, sub_class,
                            app_id, dscp_info);
 }
 }
 }
 else
 {
 if(btif_config_get_int(name, "HidAttrMask", &value))
 {
                btif_storage_remove_hid_info(&bd_addr);
                string_to_bdaddr(name, &bd_addr);
 }
 }
 }

 return BT_STATUS_SUCCESS;
}

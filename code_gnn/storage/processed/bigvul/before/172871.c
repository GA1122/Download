bt_status_t btif_storage_remove_hid_info(bt_bdaddr_t *remote_bd_addr)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));

    btif_config_remove(bdstr, "HidAttrMask");
    btif_config_remove(bdstr, "HidSubClass");
    btif_config_remove(bdstr, "HidAppId");
    btif_config_remove(bdstr, "HidVendorId");
    btif_config_remove(bdstr, "HidProductId");
    btif_config_remove(bdstr, "HidVersion");
    btif_config_remove(bdstr, "HidCountryCode");
    btif_config_remove(bdstr, "HidSSRMaxLatency");
    btif_config_remove(bdstr, "HidSSRMinTimeout");
    btif_config_remove(bdstr, "HidDescriptor");
    btif_config_save();
 return BT_STATUS_SUCCESS;
}

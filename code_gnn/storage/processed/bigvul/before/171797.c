BOOLEAN check_hid_le(const bt_bdaddr_t *remote_bdaddr)
{
 uint32_t    remote_dev_type;
 bt_property_t prop_name;

  
    BTIF_STORAGE_FILL_PROPERTY(&prop_name,BT_PROPERTY_TYPE_OF_DEVICE,
 sizeof(uint32_t), &remote_dev_type);
 if (btif_storage_get_remote_device_property((bt_bdaddr_t *)remote_bdaddr,
 &prop_name) == BT_STATUS_SUCCESS)
 {
 if (remote_dev_type == BT_DEVICE_DEVTYPE_BLE)
 {
 bdstr_t bdstr;
            bdaddr_to_string(remote_bdaddr, bdstr, sizeof(bdstr));
 if(btif_config_exist(bdstr, "HidAppId"))
 return TRUE;
 }
 }
 return FALSE;
}

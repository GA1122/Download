static bt_status_t btif_in_fetch_bonded_devices(btif_bonded_devices_t *p_bonded_devices, int add)
{
    memset(p_bonded_devices, 0, sizeof(btif_bonded_devices_t));

    BOOLEAN bt_linkkey_file_found=FALSE;
 int device_type;

 for (const btif_config_section_iter_t *iter = btif_config_section_begin(); iter != btif_config_section_end(); iter = btif_config_section_next(iter)) {
 const char *name = btif_config_section_name(iter);
 if (!string_is_bdaddr(name))
 continue;

        BTIF_TRACE_DEBUG("Remote device:%s", name);
        LINK_KEY link_key;
 size_t size = sizeof(link_key);
 if(btif_config_get_bin(name, "LinkKey", link_key, &size))
 {
 int linkkey_type;
 if(btif_config_get_int(name, "LinkKeyType", &linkkey_type))
 {
 bt_bdaddr_t bd_addr;
                string_to_bdaddr(name, &bd_addr);
 if(add)
 {
                    DEV_CLASS dev_class = {0, 0, 0};
 int cod;
 int pin_length = 0;
 if(btif_config_get_int(name, "DevClass", &cod))
                        uint2devclass((UINT32)cod, dev_class);
                    btif_config_get_int(name, "PinLength", &pin_length);
                    BTA_DmAddDevice(bd_addr.address, dev_class, link_key, 0, 0,
 (UINT8)linkkey_type, 0, pin_length);

#if BLE_INCLUDED == TRUE
 if (btif_config_get_int(name, "DevType", &device_type) &&
 (device_type == BT_DEVICE_TYPE_DUMO) )
 {
                        btif_gatts_add_bonded_dev_from_nv(bd_addr.address);
 }
#endif
 }
                bt_linkkey_file_found = TRUE;
                memcpy(&p_bonded_devices->devices[p_bonded_devices->num_devices++], &bd_addr, sizeof(bt_bdaddr_t));
 }
 else
 {
#if (BLE_INCLUDED == TRUE)
                bt_linkkey_file_found = FALSE;
#else
                BTIF_TRACE_ERROR("bounded device:%s, LinkKeyType or PinLength is invalid", name);
#endif
 }
 }
#if (BLE_INCLUDED == TRUE)
 if(!(btif_in_fetch_bonded_ble_device(name, add, p_bonded_devices)) && (!bt_linkkey_file_found))
 {
                BTIF_TRACE_DEBUG("Remote device:%s, no link key or ble key found", name);
 }
#else
 if(!bt_linkkey_file_found)
                BTIF_TRACE_DEBUG("Remote device:%s, no link key", name);
#endif
 }
 return BT_STATUS_SUCCESS;
}

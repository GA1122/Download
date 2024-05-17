static void btif_dm_cb_create_bond(bt_bdaddr_t *bd_addr, tBTA_TRANSPORT transport)
{
    BOOLEAN is_hid = check_cod(bd_addr, COD_HID_POINTING);
    bond_state_changed(BT_STATUS_SUCCESS, bd_addr, BT_BOND_STATE_BONDING);

#if BLE_INCLUDED == TRUE
 int device_type;
 int addr_type;
 bdstr_t bdstr;
    bdaddr_to_string(bd_addr, bdstr, sizeof(bdstr));
 if (transport == BT_TRANSPORT_LE)
 {
 if (!btif_config_get_int((char const *)&bdstr,"DevType", &device_type))
 {
            btif_config_set_int(bdstr, "DevType", BT_DEVICE_TYPE_BLE);
 }
 if (btif_storage_get_remote_addr_type(bd_addr, &addr_type) != BT_STATUS_SUCCESS)
 {
            btif_storage_set_remote_addr_type(bd_addr, BLE_ADDR_PUBLIC);
 }
 }
 if((btif_config_get_int((char const *)&bdstr,"DevType", &device_type) &&
 (btif_storage_get_remote_addr_type(bd_addr, &addr_type) == BT_STATUS_SUCCESS) &&
 (device_type & BT_DEVICE_TYPE_BLE) == BT_DEVICE_TYPE_BLE) || (transport == BT_TRANSPORT_LE))
 {
        BTA_DmAddBleDevice(bd_addr->address, addr_type, device_type);
 }
#endif

#if BLE_INCLUDED == TRUE
 if(is_hid && (device_type & BT_DEVICE_TYPE_BLE) == 0)
#else
 if(is_hid)
#endif
 {
 int status;
        status = btif_hh_connect(bd_addr);
 if(status != BT_STATUS_SUCCESS)
            bond_state_changed(status, bd_addr, BT_BOND_STATE_NONE);
 }
 else
 {
        BTA_DmBondByTransport((UINT8 *)bd_addr->address, transport);
 }
  
    pairing_cb.is_local_initiated = TRUE;

}

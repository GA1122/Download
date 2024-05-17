static void btif_dm_ble_passkey_req_evt(tBTA_DM_PIN_REQ *p_pin_req)
{
 bt_bdaddr_t bd_addr;
 bt_bdname_t bd_name;
    UINT32 cod;
 int dev_type;

  
 if (!btif_get_device_type(p_pin_req->bd_addr, &dev_type))
 {
        dev_type = BT_DEVICE_TYPE_BLE;
 }
    btif_dm_update_ble_remote_properties(p_pin_req->bd_addr,p_pin_req->bd_name,
 (tBT_DEVICE_TYPE) dev_type);

    bdcpy(bd_addr.address, p_pin_req->bd_addr);
    memcpy(bd_name.name, p_pin_req->bd_name, BD_NAME_LEN);

    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);
    pairing_cb.is_le_only = TRUE;

    cod = COD_UNCLASSIFIED;

    HAL_CBACK(bt_hal_cbacks, pin_request_cb,
 &bd_addr, &bd_name, cod, FALSE);
}

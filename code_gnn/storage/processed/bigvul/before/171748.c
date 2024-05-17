void btif_dm_ble_sec_req_evt(tBTA_DM_BLE_SEC_REQ *p_ble_req)
{
 bt_bdaddr_t bd_addr;
 bt_bdname_t bd_name;
    UINT32 cod;
 int dev_type;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if (pairing_cb.state == BT_BOND_STATE_BONDING)
 {
        BTIF_TRACE_DEBUG("%s Discard security request", __FUNCTION__);
 return;
 }

  
 if (!btif_get_device_type(p_ble_req->bd_addr, &dev_type))
 {
        dev_type = BT_DEVICE_TYPE_BLE;
 }
    btif_dm_update_ble_remote_properties(p_ble_req->bd_addr, p_ble_req->bd_name,
 (tBT_DEVICE_TYPE) dev_type);

    bdcpy(bd_addr.address, p_ble_req->bd_addr);
    memcpy(bd_name.name, p_ble_req->bd_name, BD_NAME_LEN);

    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);

    pairing_cb.bond_type = BOND_TYPE_PERSISTENT;
    pairing_cb.is_le_only = TRUE;
    pairing_cb.is_le_nc = FALSE;
    pairing_cb.is_ssp = TRUE;
    btm_set_bond_type_dev(p_ble_req->bd_addr, pairing_cb.bond_type);

    cod = COD_UNCLASSIFIED;

    HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name, cod,
              BT_SSP_VARIANT_CONSENT, 0);
}

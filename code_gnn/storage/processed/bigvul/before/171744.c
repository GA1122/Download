static void btif_dm_ble_key_nc_req_evt(tBTA_DM_SP_KEY_NOTIF *p_notif_req)
{
  
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

  
    btif_update_remote_properties(p_notif_req->bd_addr , p_notif_req->bd_name,
                                          NULL, BT_DEVICE_TYPE_BLE);

 bt_bdaddr_t bd_addr;
    bdcpy(bd_addr.address, p_notif_req->bd_addr);

 bt_bdname_t bd_name;
    memcpy(bd_name.name, p_notif_req->bd_name, BD_NAME_LEN);

    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);
    pairing_cb.is_ssp = FALSE;
    pairing_cb.is_le_only = TRUE;
    pairing_cb.is_le_nc = TRUE;

    HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name,
              COD_UNCLASSIFIED, BT_SSP_VARIANT_PASSKEY_CONFIRMATION,
              p_notif_req->passkey);
}

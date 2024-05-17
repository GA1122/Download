static void btif_dm_ble_key_notif_evt(tBTA_DM_SP_KEY_NOTIF *p_ssp_key_notif)
{
 bt_bdaddr_t bd_addr;
 bt_bdname_t bd_name;
    UINT32 cod;
 int dev_type;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

  
 if (!btif_get_device_type(p_ssp_key_notif->bd_addr, &dev_type))
 {
        dev_type = BT_DEVICE_TYPE_BLE;
 }
    btif_dm_update_ble_remote_properties(p_ssp_key_notif->bd_addr , p_ssp_key_notif->bd_name,
 (tBT_DEVICE_TYPE) dev_type);
    bdcpy(bd_addr.address, p_ssp_key_notif->bd_addr);
    memcpy(bd_name.name, p_ssp_key_notif->bd_name, BD_NAME_LEN);

    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);
    pairing_cb.is_ssp = FALSE;
    cod = COD_UNCLASSIFIED;

    HAL_CBACK(bt_hal_cbacks, ssp_request_cb, &bd_addr, &bd_name,
              cod, BT_SSP_VARIANT_PASSKEY_NOTIFICATION,
              p_ssp_key_notif->passkey);
}

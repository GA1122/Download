void    btif_dm_load_ble_local_keys(void)
{
    memset(&ble_local_key_cb, 0, sizeof(btif_dm_local_key_cb_t));

 if (btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_ER,(char*)&ble_local_key_cb.er[0],
                                       BT_OCTET16_LEN)== BT_STATUS_SUCCESS)
 {
        ble_local_key_cb.is_er_rcvd = TRUE;
        BTIF_TRACE_DEBUG("%s BLE ER key loaded",__FUNCTION__ );
 }

 if ((btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_IR,(char*)&ble_local_key_cb.id_keys.ir[0],
                                        BT_OCTET16_LEN)== BT_STATUS_SUCCESS )&&
 (btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_IRK, (char*)&ble_local_key_cb.id_keys.irk[0],
                                        BT_OCTET16_LEN)== BT_STATUS_SUCCESS)&&
 (btif_storage_get_ble_local_key(BTIF_DM_LE_LOCAL_KEY_DHK,(char*)&ble_local_key_cb.id_keys.dhk[0],
                                        BT_OCTET16_LEN)== BT_STATUS_SUCCESS))
 {
        ble_local_key_cb.is_id_keys_rcvd = TRUE;
        BTIF_TRACE_DEBUG("%s BLE ID keys loaded",__FUNCTION__ );
 }

}

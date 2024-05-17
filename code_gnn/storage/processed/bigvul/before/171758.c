static void btif_dm_data_free(uint16_t event, tBTA_DM_SEC *dm_sec)
{
 if (event == BTA_DM_BLE_KEY_EVT)
        osi_free(dm_sec->ble_key.p_key_value);
}

static void btif_dm_data_copy(uint16_t event, char *dst, char *src)
{
    tBTA_DM_SEC *dst_dm_sec = (tBTA_DM_SEC*)dst;
    tBTA_DM_SEC *src_dm_sec = (tBTA_DM_SEC*)src;

 if (!src_dm_sec)
 return;

    assert(dst_dm_sec);
    memcpy(dst_dm_sec, src_dm_sec, sizeof(tBTA_DM_SEC));

 if (event == BTA_DM_BLE_KEY_EVT)
 {
        dst_dm_sec->ble_key.p_key_value = osi_malloc(sizeof(tBTM_LE_KEY_VALUE));
        assert(src_dm_sec->ble_key.p_key_value);
        assert(dst_dm_sec->ble_key.p_key_value);
        memcpy(dst_dm_sec->ble_key.p_key_value, src_dm_sec->ble_key.p_key_value, sizeof(tBTM_LE_KEY_VALUE));
 }
}

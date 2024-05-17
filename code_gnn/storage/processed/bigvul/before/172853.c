static void btif_read_le_key(const uint8_t key_type, const size_t key_len, bt_bdaddr_t bd_addr,
 const uint8_t addr_type, const bool add_key, bool *device_added, bool *key_found)
{
    assert(device_added);
    assert(key_found);

 char buffer[100];
    memset(buffer, 0, sizeof(buffer));

 if (btif_storage_get_ble_bonding_key(&bd_addr, key_type, buffer, key_len) == BT_STATUS_SUCCESS)
 {
 if (add_key)
 {
            BD_ADDR bta_bd_addr;
            bdcpy(bta_bd_addr, bd_addr.address);

 if (!*device_added)
 {
                BTA_DmAddBleDevice(bta_bd_addr, addr_type, BT_DEVICE_TYPE_BLE);
 *device_added = true;
 }

 char bd_str[20] = {0};
            BTIF_TRACE_DEBUG("%s() Adding key type %d for %s", __func__,
                key_type, bdaddr_to_string(&bd_addr, bd_str, sizeof(bd_str)));
            BTA_DmAddBleKey(bta_bd_addr, (tBTA_LE_KEY_VALUE *)buffer, key_type);
 }

 *key_found = true;
 }
}

bt_status_t btif_in_fetch_bonded_ble_device(const char *remote_bd_addr, int add, btif_bonded_devices_t *p_bonded_devices)
{
 int device_type;
 int addr_type;
    UINT32 i;
 bt_bdaddr_t bd_addr;
    BD_ADDR bta_bd_addr;
 bool device_added = false;
 bool key_found = false;

 if (!btif_config_get_int(remote_bd_addr, "DevType", &device_type))
 return BT_STATUS_FAIL;

 if ((device_type & BT_DEVICE_TYPE_BLE) == BT_DEVICE_TYPE_BLE)
 {
        BTIF_TRACE_DEBUG("%s Found a LE device: %s", __func__, remote_bd_addr);

        string_to_bdaddr(remote_bd_addr, &bd_addr);
        bdcpy(bta_bd_addr, bd_addr.address);

 if (btif_storage_get_remote_addr_type(&bd_addr, &addr_type) != BT_STATUS_SUCCESS)
 {
            addr_type = BLE_ADDR_PUBLIC;
            btif_storage_set_remote_addr_type(&bd_addr, BLE_ADDR_PUBLIC);
 }

        btif_read_le_key(BTIF_DM_LE_KEY_PENC, sizeof(tBTM_LE_PENC_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

        btif_read_le_key(BTIF_DM_LE_KEY_PID, sizeof(tBTM_LE_PID_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

        btif_read_le_key(BTIF_DM_LE_KEY_LID, sizeof(tBTM_LE_PID_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

        btif_read_le_key(BTIF_DM_LE_KEY_PCSRK, sizeof(tBTM_LE_PCSRK_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

        btif_read_le_key(BTIF_DM_LE_KEY_LENC, sizeof(tBTM_LE_LENC_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

        btif_read_le_key(BTIF_DM_LE_KEY_LCSRK, sizeof(tBTM_LE_LCSRK_KEYS),
                         bd_addr, addr_type, add, &device_added, &key_found);

 if (device_added)
 {
            memcpy(&p_bonded_devices->devices[p_bonded_devices->num_devices++], &bd_addr, sizeof(bt_bdaddr_t));
            btif_gatts_add_bonded_dev_from_nv(bta_bd_addr);
 }

 if (key_found)
 return BT_STATUS_SUCCESS;
 }
 return BT_STATUS_FAIL;
}

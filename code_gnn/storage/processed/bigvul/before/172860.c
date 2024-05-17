bt_status_t btif_storage_get_ble_local_key(UINT8 key_type,
 char *key_value,
 int key_length)
{
 const char* name;
 switch(key_type)
 {
 case BTIF_DM_LE_LOCAL_KEY_IR:
            name = "LE_LOCAL_KEY_IR";
 break;
 case BTIF_DM_LE_LOCAL_KEY_IRK:
            name = "LE_LOCAL_KEY_IRK";
 break;
 case BTIF_DM_LE_LOCAL_KEY_DHK:
            name = "LE_LOCAL_KEY_DHK";
 break;
 case BTIF_DM_LE_LOCAL_KEY_ER:
            name = "LE_LOCAL_KEY_ER";
 break;
 default:
 return BT_STATUS_FAIL;
 }
 size_t length = key_length;
 int ret = btif_config_get_bin("Adapter", name, (uint8_t *)key_value, &length);
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

bt_status_t btif_storage_read_hl_apps_cb(char *value, int value_size)
{
 bt_status_t bt_status = BT_STATUS_SUCCESS;

 if (!btif_config_exist(BTIF_STORAGE_HL_APP, BTIF_STORAGE_HL_APP_CB))
 {
        memset(value, 0, value_size);
 if (!btif_config_set_bin(BTIF_STORAGE_HL_APP,BTIF_STORAGE_HL_APP_CB,
 (const uint8_t *)value, value_size))
 {
            bt_status = BT_STATUS_FAIL;
 }
 else
 {
            btif_config_save();
 }
 }
 else
 {
 size_t read_size = value_size;
 if (!btif_config_get_bin(BTIF_STORAGE_HL_APP, BTIF_STORAGE_HL_APP_CB,
 (uint8_t *)value, &read_size))
 {
            bt_status = BT_STATUS_FAIL;
 }
 else
 {
 if (read_size != (size_t)value_size)
 {
                BTIF_TRACE_ERROR("%s  value_size=%d read_size=%d",
                                  __FUNCTION__, value_size, read_size);
                bt_status = BT_STATUS_FAIL;
 }
 }

 }

    BTIF_TRACE_DEBUG("%s  status=%d value_size=%d", __FUNCTION__, bt_status, value_size);
 return bt_status;
}

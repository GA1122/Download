bt_status_t btif_storage_remove_ble_local_keys(void)
{
 int ret = 1;
 if(btif_config_exist("Adapter", "LE_LOCAL_KEY_IR"))
        ret &= btif_config_remove("Adapter", "LE_LOCAL_KEY_IR");
 if(btif_config_exist("Adapter", "LE_LOCAL_KEY_IRK"))
        ret &= btif_config_remove("Adapter", "LE_LOCAL_KEY_IRK");
 if(btif_config_exist("Adapter", "LE_LOCAL_KEY_DHK"))
        ret &= btif_config_remove("Adapter", "LE_LOCAL_KEY_DHK");
 if(btif_config_exist("Adapter", "LE_LOCAL_KEY_ER"))
        ret &= btif_config_remove("Adapter", "LE_LOCAL_KEY_ER");
    btif_config_save();
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

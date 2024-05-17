bt_status_t btif_storage_add_ble_bonding_key(bt_bdaddr_t *remote_bd_addr,
 char *key,
                                           UINT8 key_type,
                                           UINT8 key_length)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
 const char* name;
 switch(key_type)
 {
 case BTIF_DM_LE_KEY_PENC:
            name = "LE_KEY_PENC";
 break;
 case BTIF_DM_LE_KEY_PID:
            name = "LE_KEY_PID";
 break;
 case BTIF_DM_LE_KEY_PCSRK:
            name = "LE_KEY_PCSRK";
 break;
 case BTIF_DM_LE_KEY_LENC:
            name = "LE_KEY_LENC";
 break;
 case BTIF_DM_LE_KEY_LCSRK:
            name = "LE_KEY_LCSRK";
 break;
 case BTIF_DM_LE_KEY_LID:
            name = "LE_KEY_LID";
 break;
 default:
 return BT_STATUS_FAIL;
 }
 int ret = btif_config_set_bin(bdstr, name, (const uint8_t *)key, key_length);
    btif_config_save();
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

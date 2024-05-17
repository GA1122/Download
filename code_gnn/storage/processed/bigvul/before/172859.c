bt_status_t btif_storage_get_ble_bonding_key(bt_bdaddr_t *remote_bd_addr,
                                             UINT8 key_type,
 char *key_value,
 int key_length)
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
 default:
 return BT_STATUS_FAIL;
 }
 size_t length = key_length;
 int ret = btif_config_get_bin(bdstr, name, (uint8_t *)key_value, &length);
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;

}

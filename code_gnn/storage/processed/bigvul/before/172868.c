bt_status_t btif_storage_remove_ble_bonding_keys(bt_bdaddr_t *remote_bd_addr)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    BTIF_TRACE_DEBUG(" %s in bd addr:%s",__FUNCTION__, bdstr);
 int ret = 1;
 if(btif_config_exist(bdstr, "LE_KEY_PENC"))
        ret &= btif_config_remove(bdstr, "LE_KEY_PENC");
 if(btif_config_exist(bdstr, "LE_KEY_PID"))
        ret &= btif_config_remove(bdstr, "LE_KEY_PID");
 if(btif_config_exist(bdstr, "LE_KEY_PCSRK"))
        ret &= btif_config_remove(bdstr, "LE_KEY_PCSRK");
 if(btif_config_exist(bdstr, "LE_KEY_LENC"))
        ret &= btif_config_remove(bdstr, "LE_KEY_LENC");
 if(btif_config_exist(bdstr, "LE_KEY_LCSRK"))
        ret &= btif_config_remove(bdstr, "LE_KEY_LCSRK");
    btif_config_save();
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

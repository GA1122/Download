bt_status_t btif_storage_remove_bonded_device(bt_bdaddr_t *remote_bd_addr)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    BTIF_TRACE_DEBUG("in bd addr:%s", bdstr);

#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
    btif_storage_remove_ble_bonding_keys(remote_bd_addr);
#endif

 int ret = 1;
 if(btif_config_exist(bdstr, "LinkKeyType"))
        ret &= btif_config_remove(bdstr, "LinkKeyType");
 if(btif_config_exist(bdstr, "PinLength"))
        ret &= btif_config_remove(bdstr, "PinLength");
 if(btif_config_exist(bdstr, "LinkKey"))
        ret &= btif_config_remove(bdstr, "LinkKey");
  
    btif_config_flush();
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;

}

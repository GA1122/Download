bt_status_t btif_storage_set_remote_addr_type(bt_bdaddr_t *remote_bd_addr,
                                              UINT8 addr_type)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
 int ret = btif_config_set_int(bdstr, "AddrType", (int)addr_type);
    btif_config_save();
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

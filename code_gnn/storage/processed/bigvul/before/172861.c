bt_status_t btif_storage_get_remote_addr_type(bt_bdaddr_t *remote_bd_addr,
 int*addr_type)
{
 bdstr_t bdstr;
    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
 int ret = btif_config_get_int(bdstr, "AddrType", addr_type);
 return ret ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

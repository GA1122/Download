int get_remote_services(bt_bdaddr_t *remote_addr)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_get_remote_services(remote_addr);
}

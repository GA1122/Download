int get_remote_device_properties(bt_bdaddr_t *remote_addr)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_get_remote_device_properties(remote_addr);
}

int get_remote_device_property(bt_bdaddr_t *remote_addr, bt_property_type_t type)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_get_remote_device_property(remote_addr, type);
}

int set_remote_device_property(bt_bdaddr_t *remote_addr, const bt_property_t *property)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_set_remote_device_property(remote_addr, property);
}

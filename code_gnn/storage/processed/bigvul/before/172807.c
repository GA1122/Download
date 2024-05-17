static int get_adapter_property(bt_property_type_t type)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_get_adapter_property(type);
}

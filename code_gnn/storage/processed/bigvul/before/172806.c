 static int get_adapter_properties(void)
 {
      
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_get_adapter_properties();
}

static int get_connection_state(const bt_bdaddr_t *bd_addr)
{
  
 if (interface_ready() == FALSE)
 return 0;

 return btif_dm_get_connection_state(bd_addr);
}

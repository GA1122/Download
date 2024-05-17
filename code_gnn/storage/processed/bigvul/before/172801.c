static int create_bond(const bt_bdaddr_t *bd_addr, int transport)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_create_bond(bd_addr, transport);
}

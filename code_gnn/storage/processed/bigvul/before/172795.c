static int cancel_bond(const bt_bdaddr_t *bd_addr)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_cancel_bond(bd_addr);
}

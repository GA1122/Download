static int cancel_discovery(void)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_cancel_discovery();
}

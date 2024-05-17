static int start_discovery(void)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_dm_start_discovery();
}

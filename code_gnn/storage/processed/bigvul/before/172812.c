int get_remote_service_record(bt_bdaddr_t *remote_addr, bt_uuid_t *uuid)
{
  
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;

 return btif_get_remote_service_record(remote_addr, uuid);
}

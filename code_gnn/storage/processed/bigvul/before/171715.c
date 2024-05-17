bt_status_t btif_get_remote_service_record(bt_bdaddr_t *remote_addr,
 bt_uuid_t *uuid)
{
 if (!btif_is_enabled())
 return BT_STATUS_NOT_READY;

 return btif_dm_get_remote_service_record(remote_addr, uuid);
}

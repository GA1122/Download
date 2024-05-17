bt_status_t btif_set_remote_device_property(bt_bdaddr_t *remote_addr,
 const bt_property_t *property)
{
 btif_storage_req_t req;

 if (!btif_is_enabled())
 return BT_STATUS_NOT_READY;

    memcpy(&(req.write_req.bd_addr), remote_addr, sizeof(bt_bdaddr_t));
    memcpy(&(req.write_req.prop), property, sizeof(bt_property_t));

 return btif_transfer_context(execute_storage_remote_request,
                                 BTIF_CORE_STORAGE_REMOTE_WRITE,
 (char*)&req,
 sizeof(btif_storage_req_t)+property->len,
                                 btif_in_storage_request_copy_cb);
}

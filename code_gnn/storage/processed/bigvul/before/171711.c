bt_status_t btif_get_adapter_property(bt_property_type_t type)
{
 btif_storage_req_t req;

    BTIF_TRACE_EVENT("%s %d", __FUNCTION__, type);

  
 if (!btif_is_enabled() && (type != BT_PROPERTY_BDADDR) && (type != BT_PROPERTY_BDNAME))
 return BT_STATUS_NOT_READY;

    memset(&(req.read_req.bd_addr), 0, sizeof(bt_bdaddr_t));
    req.read_req.type = type;

 return btif_transfer_context(execute_storage_request,
                                 BTIF_CORE_STORAGE_ADAPTER_READ,
 (char*)&req, sizeof(btif_storage_req_t), NULL);
}
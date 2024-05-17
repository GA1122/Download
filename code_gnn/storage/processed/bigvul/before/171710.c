bt_status_t btif_get_adapter_properties(void)
{
    BTIF_TRACE_EVENT("%s", __FUNCTION__);

 if (!btif_is_enabled())
 return BT_STATUS_NOT_READY;

 return btif_transfer_context(execute_storage_request,
                                 BTIF_CORE_STORAGE_ADAPTER_READ_ALL,
                                 NULL, 0, NULL);
}

bt_status_t btif_disable_service(tBTA_SERVICE_ID service_id)
{
    tBTA_SERVICE_ID *p_id = &service_id;

  

    btif_enabled_services &= (tBTA_SERVICE_MASK)(~(1<<service_id));

    BTIF_TRACE_DEBUG("%s: Current Services:0x%x", __FUNCTION__, btif_enabled_services);

 if (btif_is_enabled())
 {
        btif_transfer_context(btif_dm_execute_service_request,
                              BTIF_DM_DISABLE_SERVICE,
 (char*)p_id, sizeof(tBTA_SERVICE_ID), NULL);
 }

 return BT_STATUS_SUCCESS;
}

bt_status_t btif_enable_service(tBTA_SERVICE_ID service_id)
{
    tBTA_SERVICE_ID *p_id = &service_id;

  

    btif_enabled_services |= (1 << service_id);

    BTIF_TRACE_DEBUG("%s: current services:0x%x", __FUNCTION__, btif_enabled_services);

 if (btif_is_enabled())
 {
        btif_transfer_context(btif_dm_execute_service_request,
                              BTIF_DM_ENABLE_SERVICE,
 (char*)p_id, sizeof(tBTA_SERVICE_ID), NULL);
 }

 return BT_STATUS_SUCCESS;
}

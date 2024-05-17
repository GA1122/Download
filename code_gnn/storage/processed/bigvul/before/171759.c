void btif_dm_execute_service_request(UINT16 event, char *p_param)
{
    BOOLEAN b_enable = FALSE;
 bt_status_t status;
 if (event == BTIF_DM_ENABLE_SERVICE)
 {
        b_enable = TRUE;
 }
    status = btif_in_execute_service_request(*((tBTA_SERVICE_ID*)p_param), b_enable);
 if (status == BT_STATUS_SUCCESS)
 {
 bt_property_t property;
 bt_uuid_t local_uuids[BT_MAX_NUM_UUIDS];

  
        BTIF_STORAGE_FILL_PROPERTY(&property, BT_PROPERTY_UUIDS,
 sizeof(local_uuids), local_uuids);
        btif_storage_get_adapter_property(&property);
        HAL_CBACK(bt_hal_cbacks, adapter_properties_cb,
                          BT_STATUS_SUCCESS, 1, &property);
 }
 return;
}

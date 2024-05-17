static void btif_dm_remote_service_record_evt(UINT16 event, char *p_param)
{
    tBTA_DM_SEARCH *p_data = (tBTA_DM_SEARCH*)p_param;

    BTIF_TRACE_EVENT("%s:  event = %d", __FUNCTION__, event);
 switch (event)
 {
 case BTA_DM_DISC_RES_EVT:
 {
 bt_service_record_t rec;
 bt_property_t prop;
 bt_bdaddr_t bd_addr;

            memset(&rec, 0, sizeof(bt_service_record_t));
            bdcpy(bd_addr.address, p_data->disc_res.bd_addr);

            BTIF_TRACE_DEBUG("%s:(result=0x%x, services 0x%x)", __FUNCTION__,
                    p_data->disc_res.result, p_data->disc_res.services);
            prop.type = BT_PROPERTY_SERVICE_RECORD;
            prop.val = (void*)&rec;
            prop.len = sizeof(rec);

  
            p_data->disc_res.services &= ~BTA_USER_SERVICE_MASK;
  
            rec.channel = p_data->disc_res.result - 3;
  
            rec.name[0] = 0;

            HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                             BT_STATUS_SUCCESS, &bd_addr, 1, &prop);
 }
 break;

 default:
 {
           ASSERTC(0, "unhandled remote service record event", event);
 }
 break;
 }
}

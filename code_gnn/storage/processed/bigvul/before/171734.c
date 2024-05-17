static void execute_storage_request(UINT16 event, char *p_param)
{
 bt_status_t status = BT_STATUS_SUCCESS;

    BTIF_TRACE_EVENT("execute storage request event : %d", event);

 switch(event)
 {
 case BTIF_CORE_STORAGE_ADAPTER_WRITE:
 {
 btif_storage_req_t *p_req = (btif_storage_req_t*)p_param;
 bt_property_t *p_prop = &(p_req->write_req.prop);
            BTIF_TRACE_EVENT("type: %d, len %d, 0x%x", p_prop->type,
                               p_prop->len, p_prop->val);

            status = btif_storage_set_adapter_property(p_prop);
            HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 1, p_prop);
 } break;

 case BTIF_CORE_STORAGE_ADAPTER_READ:
 {
 btif_storage_req_t *p_req = (btif_storage_req_t*)p_param;
 char buf[512];
 bt_property_t prop;
            prop.type = p_req->read_req.type;
            prop.val = (void*)buf;
            prop.len = sizeof(buf);
 if (prop.type == BT_PROPERTY_LOCAL_LE_FEATURES)
 {
 #if (BLE_INCLUDED == TRUE)
                tBTM_BLE_VSC_CB cmn_vsc_cb;
 bt_local_le_features_t local_le_features;

  
                BTM_BleGetVendorCapabilities(&cmn_vsc_cb);
                local_le_features.local_privacy_enabled = BTM_BleLocalPrivacyEnabled();

                prop.len = sizeof (bt_local_le_features_t);
 if (cmn_vsc_cb.filter_support == 1)
                    local_le_features.max_adv_filter_supported = cmn_vsc_cb.max_filter;
 else
                    local_le_features.max_adv_filter_supported = 0;
                local_le_features.max_adv_instance = cmn_vsc_cb.adv_inst_max;
                local_le_features.max_irk_list_size = cmn_vsc_cb.max_irk_list_sz;
                local_le_features.rpa_offload_supported = cmn_vsc_cb.rpa_offloading;
                local_le_features.scan_result_storage_size = cmn_vsc_cb.tot_scan_results_strg;
                local_le_features.activity_energy_info_supported = cmn_vsc_cb.energy_support;
                local_le_features.version_supported = cmn_vsc_cb.version_supported;
                local_le_features.total_trackable_advertisers =
                    cmn_vsc_cb.total_trackable_advertisers;

                local_le_features.extended_scan_support = cmn_vsc_cb.extended_scan_support > 0;
                local_le_features.debug_logging_supported = cmn_vsc_cb.debug_logging_supported > 0;
                memcpy(prop.val, &local_le_features, prop.len);
 #endif
 }
 else
 {
                status = btif_storage_get_adapter_property(&prop);
 }
            HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 1, &prop);
 } break;

 case BTIF_CORE_STORAGE_ADAPTER_READ_ALL:
 {
            status = btif_in_get_adapter_properties();
 } break;

 case BTIF_CORE_STORAGE_NOTIFY_STATUS:
 {
            HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, status, 0, NULL);
 } break;

 default:
            BTIF_TRACE_ERROR("%s invalid event id (%d)", __FUNCTION__, event);
 break;
 }
}

static void bte_search_devices_evt(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH *p_data)
{
    UINT16 param_len = 0;

 if (p_data)
        param_len += sizeof(tBTA_DM_SEARCH);
  
 switch (event)
 {
 case BTA_DM_INQ_RES_EVT:
 {
 if (p_data->inq_res.p_eir)
                param_len += HCI_EXT_INQ_RESPONSE_LEN;
 }
 break;

 case BTA_DM_DISC_RES_EVT:
 {
 if (p_data->disc_res.raw_data_size && p_data->disc_res.p_raw_data)
                param_len += p_data->disc_res.raw_data_size;
 }
 break;
 }
    BTIF_TRACE_DEBUG("%s event=%s param_len=%d", __FUNCTION__, dump_dm_search_event(event), param_len);

  
 if (event == BTA_DM_INQ_RES_EVT)
        p_data->inq_res.remt_name_not_required = check_eir_remote_name(p_data, NULL, NULL);

    btif_transfer_context (btif_dm_search_devices_evt , (UINT16) event, (void *)p_data, param_len,
 (param_len > sizeof(tBTA_DM_SEARCH)) ? search_devices_copy_cb : NULL);
}

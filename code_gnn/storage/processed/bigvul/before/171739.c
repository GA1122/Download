static void bte_dm_search_services_evt(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH *p_data)
{
    UINT16 param_len = 0;
 if (p_data)
       param_len += sizeof(tBTA_DM_SEARCH);
 switch (event)
 {
 case BTA_DM_DISC_RES_EVT:
 {
 if ((p_data->disc_res.result == BTA_SUCCESS) && (p_data->disc_res.num_uuids > 0)) {
                  param_len += (p_data->disc_res.num_uuids * MAX_UUID_SIZE);
 }
 } break;
 }
  
   btif_transfer_context(btif_dm_search_services_evt, event, (char*)p_data, param_len,
 (param_len > sizeof(tBTA_DM_SEARCH)) ? search_services_copy_cb : NULL);
}

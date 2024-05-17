void bte_dm_evt(tBTA_DM_SEC_EVT event, tBTA_DM_SEC *p_data)
{
  
 bt_status_t status = btif_transfer_context(btif_dm_upstreams_evt, (uint16_t)event,
 (void*)p_data, sizeof(tBTA_DM_SEC), btif_dm_data_copy);

  
    ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}

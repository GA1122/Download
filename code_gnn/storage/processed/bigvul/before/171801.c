static void bte_hh_evt(tBTA_HH_EVT event, tBTA_HH *p_data)
{
 bt_status_t status;
 int param_len = 0;

 if (BTA_HH_ENABLE_EVT == event)
        param_len = sizeof(tBTA_HH_STATUS);
 else if (BTA_HH_OPEN_EVT == event)
        param_len = sizeof(tBTA_HH_CONN);
 else if (BTA_HH_DISABLE_EVT == event)
        param_len = sizeof(tBTA_HH_STATUS);
 else if (BTA_HH_CLOSE_EVT == event)
        param_len = sizeof(tBTA_HH_CBDATA);
 else if (BTA_HH_GET_DSCP_EVT == event)
        param_len = sizeof(tBTA_HH_DEV_DSCP_INFO);
 else if ((BTA_HH_GET_PROTO_EVT == event) || (BTA_HH_GET_RPT_EVT == event)|| (BTA_HH_GET_IDLE_EVT == event))
        param_len = sizeof(tBTA_HH_HSDATA);
 else if ((BTA_HH_SET_PROTO_EVT == event) || (BTA_HH_SET_RPT_EVT == event) || (BTA_HH_VC_UNPLUG_EVT == event) || (BTA_HH_SET_IDLE_EVT == event))
        param_len = sizeof(tBTA_HH_CBDATA);
 else if ((BTA_HH_ADD_DEV_EVT == event) || (BTA_HH_RMV_DEV_EVT == event) )
        param_len = sizeof(tBTA_HH_DEV_INFO);
 else if (BTA_HH_API_ERR_EVT == event)
        param_len = 0;
  
    status = btif_transfer_context(btif_hh_upstreams_evt, (uint16_t)event, (void*)p_data, param_len, NULL);

  
    ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}

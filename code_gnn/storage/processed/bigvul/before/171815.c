void btif_hh_tmr_hdlr(TIMER_LIST_ENT *tle)
{
 btif_hh_device_t *p_dev;
    UINT8               i;
    tBTA_HH_EVT event;
    tBTA_HH p_data;
 int param_len = 0;
    memset(&p_data, 0, sizeof(tBTA_HH));

    BTIF_TRACE_DEBUG("%s timer_in_use=%d",  __FUNCTION__, tle->in_use );

 for (i = 0; i < BTIF_HH_MAX_HID; i++) {
 if (btif_hh_cb.devices[i].dev_status == BTHH_CONN_STATE_CONNECTED)
 {

            p_dev = &btif_hh_cb.devices[i];

 if (p_dev->vup_timer_active)
 {
                p_dev->vup_timer_active = FALSE;
                event = BTA_HH_VC_UNPLUG_EVT;
                p_data.dev_status.status = BTHH_ERR;
                p_data.dev_status.handle = p_dev->dev_handle;
                param_len = sizeof(tBTA_HH_CBDATA);

  
                btif_transfer_context(btif_hh_upstreams_evt, (uint16_t)event, (void*)&p_data,
                            param_len, NULL);
 }
 }
 }
}

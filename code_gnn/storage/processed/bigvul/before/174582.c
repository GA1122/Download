void  btm_keypress_notif_evt (UINT8 *p)
{
    tBTM_SP_KEYPRESS    evt_data;
    UINT8 *p_bda;

  
 if (btm_cb.api.p_sp_callback)
 {
        p_bda = evt_data.bd_addr;

        STREAM_TO_BDADDR (p_bda, p);
        evt_data.notif_type = *p;

 (*btm_cb.api.p_sp_callback) (BTM_SP_KEYPRESS_EVT, (tBTM_SP_EVT_DATA *)&evt_data);
 }
}

void btm_read_local_oob_complete (UINT8 *p)
{
    tBTM_SP_LOC_OOB evt_data;
    UINT8           status = *p++;

    BTM_TRACE_EVENT ("btm_read_local_oob_complete:%d", status);
 if (status == HCI_SUCCESS)
 {
        evt_data.status = BTM_SUCCESS;
        STREAM_TO_ARRAY16(evt_data.c, p);
        STREAM_TO_ARRAY16(evt_data.r, p);
 }
 else
        evt_data.status = BTM_ERR_PROCESSING;

 if (btm_cb.api.p_sp_callback)
 (*btm_cb.api.p_sp_callback) (BTM_SP_LOC_OOB_EVT, (tBTM_SP_EVT_DATA *)&evt_data);
}

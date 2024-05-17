void bta_hl_co_get_echo_data (UINT8 app_id, tBTA_HL_MCL_HANDLE mcl_handle,
                              UINT16 buf_size, UINT8 *p_buf,  UINT16 evt)
{
    tBTA_HL_STATUS status = BTA_HL_STATUS_FAIL;
    UNUSED(app_id);
    UNUSED(buf_size);
    UNUSED(p_buf);

    BTIF_TRACE_ERROR("%s not supported",__FUNCTION__);
    bta_hl_ci_get_echo_data(mcl_handle,  status, evt);
}

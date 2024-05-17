void btif_dm_set_oob_for_io_req(tBTA_OOB_DATA  *p_oob_data)
{
 if (oob_cb.sp_c[0] == 0 && oob_cb.sp_c[1] == 0 &&
        oob_cb.sp_c[2] == 0 && oob_cb.sp_c[3] == 0 )
 {
 *p_oob_data = FALSE;
 }
 else
 {
 *p_oob_data = TRUE;
 }
    BTIF_TRACE_DEBUG("btif_dm_set_oob_for_io_req *p_oob_data=%d", *p_oob_data);
}

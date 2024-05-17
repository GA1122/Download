void btm_sec_update_clock_offset (UINT16 handle, UINT16 clock_offset)
{
    tBTM_SEC_DEV_REC  *p_dev_rec;
    tBTM_INQ_INFO     *p_inq_info;

 if ((p_dev_rec = btm_find_dev_by_handle (handle)) == NULL)
 return;

    p_dev_rec->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;

 if ((p_inq_info = BTM_InqDbRead(p_dev_rec->bd_addr)) == NULL)
 return;

    p_inq_info->results.clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;
}

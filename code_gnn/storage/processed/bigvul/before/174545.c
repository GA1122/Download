BOOLEAN BTM_GetSecurityFlags (BD_ADDR bd_addr, UINT8 * p_sec_flags)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if ((p_dev_rec = btm_find_dev (bd_addr)) != NULL)
 {
 *p_sec_flags = (UINT8) p_dev_rec->sec_flags;
 return(TRUE);
 }
    BTM_TRACE_ERROR ("BTM_GetSecurityFlags false");
 return(FALSE);
}

BOOLEAN BTM_GetSecurityFlagsByTransport (BD_ADDR bd_addr, UINT8 * p_sec_flags,
                                                tBT_TRANSPORT transport)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if ((p_dev_rec = btm_find_dev (bd_addr)) != NULL)
 {
 if (transport == BT_TRANSPORT_BR_EDR)
 *p_sec_flags = (UINT8) p_dev_rec->sec_flags;
 else
 *p_sec_flags = (UINT8) (p_dev_rec->sec_flags >> 8);

 return(TRUE);
 }
    BTM_TRACE_ERROR ("BTM_GetSecurityFlags false");
 return(FALSE);
}

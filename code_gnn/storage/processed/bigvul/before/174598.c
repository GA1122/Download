UINT8 btm_sec_clr_service_by_psm (UINT16 psm)
{
    tBTM_SEC_SERV_REC   *p_srec = &btm_cb.sec_serv_rec[0];
    UINT8   num_freed = 0;
 int     i;

 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
 {
  
 if ((p_srec->security_flags & BTM_SEC_IN_USE) && (p_srec->psm == psm) )
 {
            BTM_TRACE_API("BTM_SEC_CLR[%d]: id %d ", i, p_srec->service_id);
            p_srec->security_flags = 0;
            num_freed++;
 }
 }
    BTM_TRACE_API("btm_sec_clr_service_by_psm psm:0x%x num_freed:%d", psm, num_freed);

 return(num_freed);
}

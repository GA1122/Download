UINT8 BTM_SecClrUCDService (UINT8 service_id)
{
#if (L2CAP_UCD_INCLUDED == TRUE)
    tBTM_SEC_SERV_REC   *p_srec = &btm_cb.sec_serv_rec[0];
    UINT8   num_cleared = 0;
 int     i;

 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
 {
  
 if ((p_srec->security_flags & BTM_SEC_IN_USE) &&
 (!service_id || (service_id == (UINT32)p_srec->service_id)))
 {
            BTM_TRACE_API("BTM_UCD_SEC_CLR[%d]: id %d", i, service_id);
            p_srec->ucd_security_flags = 0;
            num_cleared++;
 }
 }

 return(num_cleared);
#else
    UNUSED(service_id);
 return(0);
#endif
}

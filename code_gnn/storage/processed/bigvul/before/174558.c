UINT8 BTM_SecClrService (UINT8 service_id)
{
    tBTM_SEC_SERV_REC   *p_srec = &btm_cb.sec_serv_rec[0];
    UINT8   num_freed = 0;
 int     i;

 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++)
 {
  
 if ((p_srec->security_flags & BTM_SEC_IN_USE) && (p_srec->psm != BT_PSM_SDP) &&
 (!service_id || (service_id == p_srec->service_id)))
 {
            BTM_TRACE_API("BTM_SEC_CLR[%d]: id %d", i, service_id);
            p_srec->security_flags = 0;
#if (L2CAP_UCD_INCLUDED == TRUE)
            p_srec->ucd_security_flags = 0;
#endif
            num_freed++;
 }
 }

 return(num_freed);
}

static tBTM_SEC_SERV_REC *btm_sec_find_first_serv (CONNECTION_TYPE conn_type, UINT16 psm)
{
    tBTM_SEC_SERV_REC *p_serv_rec = &btm_cb.sec_serv_rec[0];
 int i;
    BOOLEAN is_originator;

#if (L2CAP_UCD_INCLUDED == TRUE)

 if ( conn_type & CONNECTION_TYPE_ORIG_MASK )
        is_originator = TRUE;
 else
        is_originator = FALSE;
#else
    is_originator = conn_type;
#endif

 if (is_originator && btm_cb.p_out_serv && btm_cb.p_out_serv->psm == psm)
 {
  
 return btm_cb.p_out_serv;
 }

  
 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++)
 {
 if ( (p_serv_rec->security_flags & BTM_SEC_IN_USE) && (p_serv_rec->psm == psm) )
 return(p_serv_rec);
 }
 return(NULL);
}

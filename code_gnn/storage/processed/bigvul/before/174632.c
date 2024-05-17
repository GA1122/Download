static BOOLEAN btm_sec_set_security_level (CONNECTION_TYPE conn_type, char *p_name, UINT8 service_id,
                                           UINT16 sec_level, UINT16 psm, UINT32 mx_proto_id,
                                           UINT32 mx_chan_id)
{
    tBTM_SEC_SERV_REC   *p_srec;
    UINT16               index;
    UINT16               first_unused_record = BTM_NO_AVAIL_SEC_SERVICES;
    BOOLEAN              record_allocated = FALSE;
    BOOLEAN              is_originator;
#if (L2CAP_UCD_INCLUDED == TRUE)
    BOOLEAN              is_ucd;

 if (conn_type & CONNECTION_TYPE_ORIG_MASK)
        is_originator = TRUE;
 else
        is_originator = FALSE;

 if (conn_type & CONNECTION_TYPE_CONNLESS_MASK )
 {
        is_ucd = TRUE;
 }
 else
 {
        is_ucd = FALSE;
 }
#else
    is_originator = conn_type;
#endif

  

    p_srec = &btm_cb.sec_serv_rec[0];


 for (index = 0; index < BTM_SEC_MAX_SERVICE_RECORDS; index++, p_srec++)
 {
  
 if (p_srec->security_flags & BTM_SEC_IN_USE)
 {
#if BTM_SEC_SERVICE_NAME_LEN > 0
 if (p_srec->psm == psm                  &&
                p_srec->mx_proto_id == mx_proto_id  &&
                service_id == p_srec->service_id    &&
 (!strncmp (p_name, (char *) p_srec->orig_service_name,
                           BTM_SEC_SERVICE_NAME_LEN) ||
 !strncmp (p_name, (char *) p_srec->term_service_name,
                           BTM_SEC_SERVICE_NAME_LEN)))
#else
 if (p_srec->psm == psm                  &&
                p_srec->mx_proto_id == mx_proto_id  &&
                service_id == p_srec->service_id)
#endif
 {
                record_allocated = TRUE;
 break;
 }
 }
  
 else if (!record_allocated)
 {
            memset (p_srec, 0, sizeof(tBTM_SEC_SERV_REC));
            record_allocated = TRUE;
            first_unused_record = index;
 }
 }

 if (!record_allocated)
 {
        BTM_TRACE_WARNING("BTM_SEC_REG: Out of Service Records (%d)",  BTM_SEC_MAX_SERVICE_RECORDS);
 return(record_allocated);
 }

  
  
 if (index >= BTM_SEC_MAX_SERVICE_RECORDS)
 {
        index = first_unused_record;
        p_srec = &btm_cb.sec_serv_rec[index];
 }

    p_srec->psm         = psm;
    p_srec->service_id  = service_id;
    p_srec->mx_proto_id = mx_proto_id;

 if (is_originator)
 {
        p_srec->orig_mx_chan_id = mx_chan_id;
#if BTM_SEC_SERVICE_NAME_LEN > 0
        BCM_STRNCPY_S ((char *)p_srec->orig_service_name, sizeof(p_srec->orig_service_name), p_name, BTM_SEC_SERVICE_NAME_LEN);
#endif
  
#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( is_ucd )
 {
            p_srec->ucd_security_flags &=
 ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_ENCRYPT    | BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_MITM |
              BTM_SEC_FORCE_MASTER | BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);
 }
 else
#endif
 {
            p_srec->security_flags &=
 ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_ENCRYPT    | BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_MITM |
              BTM_SEC_FORCE_MASTER | BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);
 }

  
        sec_level &= ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_MITM);

 if (btm_cb.security_mode == BTM_SEC_MODE_SP)
 {
 if (sec_level & BTM_SEC_OUT_AUTHENTICATE)
                sec_level |= BTM_SEC_OUT_MITM;
 }

  
 if (sec_level & BTM_SEC_OUT_ENCRYPT)
            sec_level |= BTM_SEC_OUT_AUTHENTICATE;

  
#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( is_ucd == FALSE )
#endif
 {
            btm_cb.p_out_serv = p_srec;
 }
 }
 else
 {
        p_srec->term_mx_chan_id = mx_chan_id;
#if BTM_SEC_SERVICE_NAME_LEN > 0
        BCM_STRNCPY_S ((char *)p_srec->term_service_name, sizeof(p_srec->term_service_name), p_name, BTM_SEC_SERVICE_NAME_LEN);
#endif
  
#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( is_ucd )
 {
            p_srec->ucd_security_flags &=
 ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT     | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_MITM |
              BTM_SEC_FORCE_MASTER | BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);
 }
 else
#endif
 {
            p_srec->security_flags &=
 ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT     | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_MITM |
              BTM_SEC_FORCE_MASTER | BTM_SEC_ATTEMPT_MASTER | BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);
 }

  
        sec_level &= ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_OUT_ENCRYPT | BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_MITM);

 if (btm_cb.security_mode == BTM_SEC_MODE_SP)
 {
 if (sec_level & BTM_SEC_IN_AUTHENTICATE)
                sec_level |= BTM_SEC_IN_MITM;
 }

  
 if (sec_level & BTM_SEC_IN_ENCRYPT)
            sec_level |= BTM_SEC_IN_AUTHENTICATE;
 }

#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( is_ucd )
 {
        p_srec->security_flags     |= (UINT16)(BTM_SEC_IN_USE);
        p_srec->ucd_security_flags |= (UINT16)(sec_level | BTM_SEC_IN_USE);
 }
 else
 {
        p_srec->security_flags |= (UINT16)(sec_level | BTM_SEC_IN_USE);
 }

    BTM_TRACE_API("BTM_SEC_REG[%d]: id %d, conn_type 0x%x, psm 0x%04x, proto_id %d, chan_id %d",
                   index, service_id, conn_type, psm, mx_proto_id, mx_chan_id);

    BTM_TRACE_API("               : security_flags: 0x%04x, ucd_security_flags: 0x%04x",
                   p_srec->security_flags, p_srec->ucd_security_flags);

#if BTM_SEC_SERVICE_NAME_LEN > 0
    BTM_TRACE_API("               : service name [%s] (up to %d chars saved)",
                   p_name, BTM_SEC_SERVICE_NAME_LEN);
#endif
#else
    p_srec->security_flags |= (UINT16)(sec_level | BTM_SEC_IN_USE);

    BTM_TRACE_API("BTM_SEC_REG[%d]: id %d, is_orig %d, psm 0x%04x, proto_id %d, chan_id %d",
                   index, service_id, is_originator, psm, mx_proto_id, mx_chan_id);

#if BTM_SEC_SERVICE_NAME_LEN > 0
    BTM_TRACE_API("               : sec: 0x%x, service name [%s] (up to %d chars saved)",
                   p_srec->security_flags, p_name, BTM_SEC_SERVICE_NAME_LEN);
#endif
#endif


 return(record_allocated);
}

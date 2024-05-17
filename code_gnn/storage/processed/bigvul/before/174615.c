static tBTM_SEC_SERV_REC *btm_sec_find_mx_serv (UINT8 is_originator, UINT16 psm,
                                                UINT32 mx_proto_id, UINT32 mx_chan_id)
{
    tBTM_SEC_SERV_REC *p_out_serv = btm_cb.p_out_serv;
    tBTM_SEC_SERV_REC *p_serv_rec = &btm_cb.sec_serv_rec[0];
 int i;

    BTM_TRACE_DEBUG ("btm_sec_find_mx_serv");
 if (is_originator && p_out_serv && p_out_serv->psm == psm
 && p_out_serv->mx_proto_id == mx_proto_id
 && p_out_serv->orig_mx_chan_id == mx_chan_id)
 {
  
 return btm_cb.p_out_serv;
 }

  
 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++)
 {
 if ((p_serv_rec->security_flags & BTM_SEC_IN_USE)
 && (p_serv_rec->psm == psm)
 && (p_serv_rec->mx_proto_id == mx_proto_id)
 && (( is_originator && (p_serv_rec->orig_mx_chan_id  == mx_chan_id))
 || (!is_originator && (p_serv_rec->term_mx_chan_id  == mx_chan_id))))
 {
 return(p_serv_rec);
 }
 }
 return(NULL);
}

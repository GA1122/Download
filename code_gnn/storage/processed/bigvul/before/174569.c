void BTM_SetOutService(BD_ADDR bd_addr, UINT8 service_id, UINT32 mx_chan_id)
{
    tBTM_SEC_DEV_REC *p_dev_rec;
    tBTM_SEC_SERV_REC *p_serv_rec = &btm_cb.sec_serv_rec[0];
 int i;

    btm_cb.p_out_serv = p_serv_rec;
    p_dev_rec = btm_find_dev (bd_addr);

 for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++)
 {
 if ((p_serv_rec->security_flags & BTM_SEC_IN_USE)
 && (p_serv_rec->service_id == service_id)
 && (p_serv_rec->orig_mx_chan_id == mx_chan_id))
 {
            BTM_TRACE_API("BTM_SetOutService p_out_serv id %d, psm 0x%04x, proto_id %d, chan_id %d",
                           p_serv_rec->service_id, p_serv_rec->psm, p_serv_rec->mx_proto_id, p_serv_rec->orig_mx_chan_id);
            btm_cb.p_out_serv = p_serv_rec;
 if (p_dev_rec)
                p_dev_rec->p_cur_service = p_serv_rec;
 break;
 }
 }
}

static BOOLEAN btm_sec_is_upgrade_possible(tBTM_SEC_DEV_REC  *p_dev_rec, BOOLEAN is_originator)
{
    UINT16              mtm_check = is_originator ? BTM_SEC_OUT_MITM : BTM_SEC_IN_MITM;
    BOOLEAN             is_possible = TRUE;

 if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)
 {
        is_possible = FALSE;
 if(p_dev_rec->p_cur_service)
 {
        BTM_TRACE_DEBUG ("btm_sec_is_upgrade_possible id:%d, link_key_typet:%d, rmt_io_caps:%d, chk flags:x%x, flags:x%x",
                          p_dev_rec->p_cur_service->service_id, p_dev_rec->link_key_type, p_dev_rec->rmt_io_caps,
                          mtm_check, p_dev_rec->p_cur_service->security_flags);
 }
 else
 {
            BTM_TRACE_DEBUG ("btm_sec_is_upgrade_possible link_key_typet:%d, rmt_io_caps:%d, chk flags:x%x, ",
                          p_dev_rec->link_key_type, p_dev_rec->rmt_io_caps, mtm_check);
 }
  
 if ((p_dev_rec->security_required & mtm_check)  
 && (p_dev_rec->link_key_type == BTM_LKEY_TYPE_UNAUTH_COMB)  
 && (p_dev_rec->rmt_io_caps < BTM_IO_CAP_MAX)  
 && (btm_sec_io_map[p_dev_rec->rmt_io_caps][btm_cb.devcb.loc_io_caps]))  
 {
  
            is_possible = TRUE;
 }
 }
    BTM_TRACE_DEBUG ("btm_sec_is_upgrade_possible is_possible:%d sec_flags:0x%x", is_possible, p_dev_rec->sec_flags);
 return is_possible;
}

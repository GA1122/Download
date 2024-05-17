tBTM_STATUS btm_sec_mx_access_request (BD_ADDR bd_addr, UINT16 psm, BOOLEAN is_originator,
                                       UINT32 mx_proto_id, UINT32 mx_chan_id,
                                       tBTM_SEC_CALLBACK *p_callback, void *p_ref_data)

{
    tBTM_SEC_DEV_REC  *p_dev_rec;
    tBTM_SEC_SERV_REC *p_serv_rec;
    tBTM_STATUS        rc;
    UINT16             security_required;
    BOOLEAN transport   = FALSE; 

    BTM_TRACE_DEBUG ("btm_sec_mx_access_request is_originator:%d", is_originator);
  
    p_dev_rec = btm_find_or_alloc_dev (bd_addr);

  
    p_serv_rec = btm_sec_find_mx_serv (is_originator, psm, mx_proto_id, mx_chan_id);

  
 if (!p_serv_rec)
 {
 if (p_callback)
 (*p_callback) (bd_addr, transport, p_ref_data, BTM_MODE_UNSUPPORTED);

        BTM_TRACE_ERROR ("Security Manager: MX service not found PSM:%d Proto:%d SCN:%d",
                          psm, mx_proto_id, mx_chan_id);
 return BTM_NO_RESOURCES;
 }

  
  
 if ( (p_dev_rec->p_callback) || (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) )
 {
        BTM_TRACE_EVENT ("btm_sec_mx_access_request service PSM:%d Proto:%d SCN:%d delayed  state: %s",
                          psm, mx_proto_id, mx_chan_id, btm_pair_state_descr(btm_cb.pairing_state));

        rc = BTM_CMD_STARTED;
        security_required = p_serv_rec->security_flags;
 if ((BTM_SEC_MODE_SP != btm_cb.security_mode)
 || ((BTM_SEC_MODE_SP == btm_cb.security_mode) && (BTM_SM4_KNOWN == p_dev_rec->sm4))
 || (BTM_SEC_IS_SM4(p_dev_rec->sm4) && (btm_sec_is_upgrade_possible(p_dev_rec, is_originator) == FALSE))
 )
 {
  
 if (is_originator)
 {
 if (((security_required & BTM_SEC_OUT_FLAGS) == 0) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_AUTHENTICATE) && btm_dev_authenticated(p_dev_rec))) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT)) && btm_dev_encrypted(p_dev_rec)))
 )
 {
                    rc = BTM_SUCCESS;
 }
 }
 else
 {
 if (((security_required & BTM_SEC_IN_FLAGS) == 0) ||
 ((((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHENTICATE) && btm_dev_authenticated(p_dev_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHORIZE) && (btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE)) && ((btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec)) && btm_dev_authenticated(p_dev_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_ENCRYPT)) && ((btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec))&& btm_dev_encrypted(p_dev_rec))) ||
 ((((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)) && btm_dev_encrypted(p_dev_rec)))
 )
 {
                    rc = BTM_SUCCESS;
 }
 }
 }
 if (rc == BTM_CMD_STARTED)
 {
            btm_sec_queue_mx_request (bd_addr, psm,  is_originator, mx_proto_id, mx_chan_id, p_callback, p_ref_data);
 return rc;
 }
 }

    p_dev_rec->p_cur_service     = p_serv_rec;
    p_dev_rec->security_required = p_serv_rec->security_flags;

 if (BTM_SEC_MODE_SP == btm_cb.security_mode)
 {
 if (BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
  
            btm_sec_check_upgrade(p_dev_rec, is_originator);
 }
 }

    p_dev_rec->is_originator     = is_originator;
    p_dev_rec->p_callback        = p_callback;
    p_dev_rec->p_ref_data        = p_ref_data;

  
  
  
  
    p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED);

    BTM_TRACE_EVENT ("Security Manager: mx_access_req proto_id:%d chan_id:%d State:%d Flags:0x%x Required:0x%x Service ID:%d",
                      mx_proto_id, mx_chan_id, p_dev_rec->sec_state, p_dev_rec->sec_flags, p_dev_rec->security_required, p_dev_rec->p_cur_service->service_id);

 if ((rc = btm_sec_execute_procedure (p_dev_rec)) != BTM_CMD_STARTED)
 {
 if (p_callback)
 {
            p_dev_rec->p_callback = NULL;

 (*p_callback) (bd_addr,transport, p_ref_data, (UINT8)rc);
 }
 }

 return rc;
}

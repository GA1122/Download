tBTM_STATUS btm_sec_l2cap_access_req (BD_ADDR bd_addr, UINT16 psm, UINT16 handle,
                                      CONNECTION_TYPE conn_type,
                                      tBTM_SEC_CALLBACK *p_callback,
 void *p_ref_data)
{
    tBTM_SEC_DEV_REC  *p_dev_rec;
    tBTM_SEC_SERV_REC *p_serv_rec;
    UINT16         security_required;
    UINT16         old_security_required;
    BOOLEAN       old_is_originator;
    tBTM_STATUS   rc = BTM_SUCCESS;
    BOOLEAN       chk_acp_auth_done = FALSE;
    BOOLEAN is_originator;
    BOOLEAN     transport = FALSE;  

#if (L2CAP_UCD_INCLUDED == TRUE)
 if (conn_type & CONNECTION_TYPE_ORIG_MASK)
        is_originator = TRUE;
 else
        is_originator = FALSE;

    BTM_TRACE_DEBUG ("btm_sec_l2cap_access_req conn_type:0x%x, 0x%x", conn_type, p_ref_data);
#else
    is_originator = conn_type;

    BTM_TRACE_DEBUG ("btm_sec_l2cap_access_req is_originator:%d, 0x%x", is_originator, p_ref_data);
#endif

  
    p_dev_rec = btm_find_or_alloc_dev (bd_addr);

    p_dev_rec->hci_handle = handle;

  
    p_serv_rec = btm_sec_find_first_serv (conn_type, psm);

  
 if (!p_serv_rec)
 {
        BTM_TRACE_WARNING ("btm_sec_l2cap_access_req()  PSM:%d no application registerd", psm);

 (*p_callback) (bd_addr, transport, p_ref_data, BTM_MODE_UNSUPPORTED);

 return(BTM_MODE_UNSUPPORTED);
 }

  
 if (BT_PSM_SDP == psm)
 {
 (*p_callback) (bd_addr,transport, p_ref_data, BTM_SUCCESS_NO_SECURITY);

 return(BTM_SUCCESS);
 }
#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( conn_type & CONNECTION_TYPE_CONNLESS_MASK )
 {
        security_required = p_serv_rec->ucd_security_flags;

        rc = BTM_CMD_STARTED;
 if (is_originator)
 {
 if (((security_required & BTM_SEC_OUT_FLAGS) == 0) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_AUTHENTICATE) && (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT)) && (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED))) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_FLAGS) && (p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED))) )
 {
                rc = BTM_SUCCESS;
 }
 }
 else
 {
 if (((security_required & BTM_SEC_IN_FLAGS) == 0) ||
 ((((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHENTICATE) && (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))) ||
 ((((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)) && (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED))) ||
 ((((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_FLAGS) && (p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED))) )
 {
                rc = BTM_SUCCESS;
 }
 }

 if (rc == BTM_SUCCESS)
 {
 if (p_callback)
 (*p_callback) (bd_addr, transport, (void *)p_ref_data, BTM_SUCCESS);

 return(BTM_SUCCESS);
 }
 }
 else
#endif
 {
        security_required = p_serv_rec->security_flags;
 }

  
  
 if ( (p_dev_rec->p_callback) || (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE) )
 {
        BTM_TRACE_EVENT ("btm_sec_l2cap_access_req() - busy - PSM:%d delayed  state: %s mode:%d, sm4:0x%x",
                          psm, btm_pair_state_descr(btm_cb.pairing_state), btm_cb.security_mode, p_dev_rec->sm4);
        BTM_TRACE_EVENT ("security_flags:x%x, sec_flags:x%x", security_required, p_dev_rec->sec_flags);
        rc = BTM_CMD_STARTED;
 if ((BTM_SEC_MODE_SP != btm_cb.security_mode)
 || ((BTM_SEC_MODE_SP == btm_cb.security_mode) && (BTM_SM4_KNOWN == p_dev_rec->sm4))
 || (BTM_SEC_IS_SM4(p_dev_rec->sm4) && (btm_sec_is_upgrade_possible(p_dev_rec, is_originator) == FALSE))
 )
 {
  
 if (is_originator)
 {
 if (((security_required & BTM_SEC_OUT_FLAGS) == 0) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_AUTHENTICATE) && btm_dev_authenticated(p_dev_rec))) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT)) && btm_dev_encrypted(p_dev_rec))) ||
 ((((security_required & BTM_SEC_OUT_FLAGS) == BTM_SEC_OUT_FLAGS) && btm_dev_authorized(p_dev_rec) && btm_dev_encrypted(p_dev_rec))) )
 {
                    rc = BTM_SUCCESS;
 }
 }
 else
 {
 if (((security_required & BTM_SEC_IN_FLAGS) == 0) ||
 (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHENTICATE) && btm_dev_authenticated(p_dev_rec)) ||
 (((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)) && btm_dev_encrypted(p_dev_rec)) ||
 (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_AUTHORIZE) && (btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_AUTHORIZE)) && ((btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec)) && btm_dev_authenticated(p_dev_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == (BTM_SEC_IN_ENCRYPT | BTM_SEC_IN_AUTHORIZE)) && ((btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec)) && btm_dev_encrypted(p_dev_rec))) ||
 (((security_required & BTM_SEC_IN_FLAGS) == BTM_SEC_IN_FLAGS) && btm_dev_encrypted(p_dev_rec) && (btm_dev_authorized(p_dev_rec)||btm_serv_trusted(p_dev_rec, p_serv_rec))))
 {
                    rc = BTM_SUCCESS;
 }
 }

 if (rc == BTM_SUCCESS)
 {
 if (p_callback)
 (*p_callback) (bd_addr, transport, (void *)p_ref_data, BTM_SUCCESS);

 return(BTM_SUCCESS);
 }
 }

        btm_cb.sec_req_pending = TRUE;
 return(BTM_CMD_STARTED);
 }

  
    p_dev_rec->p_cur_service = p_serv_rec;


  
 if (btm_cb.security_mode == BTM_SEC_MODE_SP)
 {
 if (is_originator)
 {
 if (BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
  
                security_required |= (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT);
 }
 else
 {
 if ( !(BTM_SM4_KNOWN & p_dev_rec->sm4))
 {
                    BTM_TRACE_DEBUG ("remote features unknown!!sec_flags:0x%x", p_dev_rec->sec_flags);
  
                    p_dev_rec->sm4          |= BTM_SM4_REQ_PEND;

 return(BTM_CMD_STARTED);
 }
 }
 }
 else
 {
  
 if (BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
  
                chk_acp_auth_done = TRUE;
  
                security_required |= (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT);
 }
 else
 {
 if ( !(BTM_SM4_KNOWN & p_dev_rec->sm4))
 {
                    BTM_TRACE_DEBUG ("(rsp) remote features unknown!!sec_flags:0x%x", p_dev_rec->sec_flags);
  
                    p_dev_rec->sm4          |= BTM_SM4_REQ_PEND;

 return(BTM_CMD_STARTED);
 }
 }
 }
 }

    BTM_TRACE_DEBUG ("btm_sec_l2cap_access_req()  sm4:0x%x, sec_flags:0x%x, security_required:0x%x chk:%d",
                      p_dev_rec->sm4, p_dev_rec->sec_flags, security_required, chk_acp_auth_done);

    old_security_required        = p_dev_rec->security_required;
    old_is_originator            = p_dev_rec->is_originator;
    p_dev_rec->security_required = security_required;
    p_dev_rec->p_ref_data        = p_ref_data;
    p_dev_rec->is_originator     = is_originator;

#if (L2CAP_UCD_INCLUDED == TRUE)
 if ( conn_type & CONNECTION_TYPE_CONNLESS_MASK )
        p_dev_rec->is_ucd = TRUE;
 else
        p_dev_rec->is_ucd = FALSE;
#endif

  
  
#if (L2CAP_UCD_INCLUDED == TRUE)
 if (((btm_sec_find_next_serv (p_serv_rec)) != NULL)
 &&(!( conn_type & CONNECTION_TYPE_CONNLESS_MASK )))  
#else
 if ((btm_sec_find_next_serv (p_serv_rec)) != NULL)
#endif
 {
        BTM_TRACE_DEBUG ("no next_serv sm4:0x%x, chk:%d", p_dev_rec->sm4, chk_acp_auth_done);
 if (!BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
            BTM_TRACE_EVENT ("Security Manager: l2cap_access_req PSM:%d postponed for multiplexer", psm);
  
            p_dev_rec->security_required = old_security_required;
            p_dev_rec->is_originator     = old_is_originator;

 (*p_callback) (bd_addr, transport, p_ref_data, BTM_SUCCESS);

 return(BTM_SUCCESS);
 }
 }

  
 if (is_originator && (btm_cb.security_mode != BTM_SEC_MODE_SP || !BTM_SEC_IS_SM4(p_dev_rec->sm4)) && (psm >= 0x1001))
 {
        BTM_TRACE_EVENT ("dynamic PSM:0x%x in legacy mode - postponed for upper layer", psm);
  
        p_dev_rec->security_required = old_security_required;
        p_dev_rec->is_originator     = old_is_originator;

 (*p_callback) (bd_addr, transport, p_ref_data, BTM_SUCCESS);

 return(BTM_SUCCESS);
 }

 if (chk_acp_auth_done)
 {
        BTM_TRACE_DEBUG ("(SM4 to SM4) btm_sec_l2cap_access_req rspd. authenticated: x%x, enc: x%x",
 (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED), (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED));
  
 if ((0 == (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
 ||(0 == (p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED)))
 {
            rc = BTM_DELAY_CHECK;
  
            BTM_TRACE_ERROR ("peer should have initiated security process by now (SM4 to SM4)");
            p_dev_rec->p_callback        = p_callback;
            p_dev_rec->sec_state         = BTM_SEC_STATE_DELAY_FOR_ENC;
 (*p_callback) (bd_addr, transport, p_ref_data, rc);

 return(BTM_CMD_STARTED);
 }
 }

    p_dev_rec->p_callback        = p_callback;

 if (p_dev_rec->last_author_service_id == BTM_SEC_NO_LAST_SERVICE_ID
 || p_dev_rec->last_author_service_id != p_dev_rec->p_cur_service->service_id)
 {
  
        p_dev_rec->sec_flags &= ~BTM_SEC_AUTHORIZED;
 }

 if (BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
  
        btm_sec_check_upgrade(p_dev_rec, is_originator);
 }

    BTM_TRACE_EVENT ("Security Manager: l2cap_access_req PSM:%d Handle:%d State:%d Flags:0x%x Required:0x%x Service ID:%d",
                      psm, handle, p_dev_rec->sec_state, p_dev_rec->sec_flags, p_dev_rec->security_required, p_dev_rec->p_cur_service->service_id);

 if ((rc = btm_sec_execute_procedure (p_dev_rec)) != BTM_CMD_STARTED)
 {
        p_dev_rec->p_callback = NULL;
 (*p_callback) (bd_addr, transport, p_dev_rec->p_ref_data, (UINT8)rc);
 }

 return(rc);
}

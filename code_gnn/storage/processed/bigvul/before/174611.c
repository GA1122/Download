static tBTM_STATUS btm_sec_execute_procedure (tBTM_SEC_DEV_REC *p_dev_rec)
{
    BTM_TRACE_EVENT ("btm_sec_execute_procedure: Required:0x%x Flags:0x%x State:%d",
                      p_dev_rec->security_required, p_dev_rec->sec_flags, p_dev_rec->sec_state);

  
 if (p_dev_rec->sec_state != 0)
 return(BTM_CMD_STARTED);

  
 if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN)
 && (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE))
 {
        BTM_TRACE_EVENT ("Security Manager: Start get name");
 if (!btm_sec_start_get_name (p_dev_rec))
 {
 return(BTM_NO_RESOURCES);
 }
 return(BTM_CMD_STARTED);
 }

  
  
 if ((!(p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
 && (( p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE))
 || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_AUTHENTICATE)))
 && (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE))
 {
#if (L2CAP_UCD_INCLUDED == TRUE)
  
 if ( !p_dev_rec->is_originator && (p_dev_rec->is_ucd == TRUE ))
 return(BTM_FAILED_ON_SECURITY);
#endif

        BTM_TRACE_EVENT ("Security Manager: Start authentication");

 if (!btm_sec_start_authentication (p_dev_rec))
 {
 return(BTM_NO_RESOURCES);
 }
 return(BTM_CMD_STARTED);
 }

  
  
 if (!(p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED)
 && (( p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_ENCRYPT))
 || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_ENCRYPT)))
 && (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE))
 {
#if (L2CAP_UCD_INCLUDED == TRUE)
  
 if ( !p_dev_rec->is_originator && (p_dev_rec->is_ucd == TRUE ))
 return(BTM_FAILED_ON_SECURITY);
#endif

        BTM_TRACE_EVENT ("Security Manager: Start encryption");

 if (!btm_sec_start_encryption (p_dev_rec))
 {
 return(BTM_NO_RESOURCES);
 }
 return(BTM_CMD_STARTED);
 }

  
  
 if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED)
 && (( p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_AUTHORIZE))
 || (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_AUTHORIZE))))
 {
        BTM_TRACE_EVENT ("service id:%d, is trusted:%d",
                          p_dev_rec->p_cur_service->service_id,
 (BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask,
                                                      p_dev_rec->p_cur_service->service_id)));
 if ((btm_sec_are_all_trusted(p_dev_rec->trusted_mask) == FALSE) &&
 (p_dev_rec->p_cur_service->service_id < BTM_SEC_MAX_SERVICES) &&
 (BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask,
                                        p_dev_rec->p_cur_service->service_id) == FALSE))
 {
            BTM_TRACE_EVENT ("Security Manager: Start authorization");
 return(btm_sec_start_authorization (p_dev_rec));
 }
 }

  
    p_dev_rec->security_required &= ~(BTM_SEC_OUT_AUTHORIZE | BTM_SEC_IN_AUTHORIZE |
                                      BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_IN_AUTHENTICATE |
                                      BTM_SEC_OUT_ENCRYPT | BTM_SEC_IN_ENCRYPT |
                                      BTM_SEC_FORCE_MASTER | BTM_SEC_ATTEMPT_MASTER |
                                      BTM_SEC_FORCE_SLAVE | BTM_SEC_ATTEMPT_SLAVE);

    BTM_TRACE_EVENT ("Security Manager: trusted:0x%04x%04x", p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0]);
    BTM_TRACE_EVENT ("Security Manager: access granted");

 return(BTM_SUCCESS);
}

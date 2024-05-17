void smp_decide_association_model(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t int_evt = 0;
  tSMP_INT_DATA smp_int_data;

  SMP_TRACE_DEBUG("%s Association Model = %d", __func__,
                  p_cb->selected_association_model);

 switch (p_cb->selected_association_model) {
 case SMP_MODEL_ENCRYPTION_ONLY:  
 if (p_cb->role == HCI_ROLE_MASTER &&
 ((p_cb->peer_auth_req & SMP_AUTH_YN_BIT) != 0) &&
 ((p_cb->loc_auth_req & SMP_AUTH_YN_BIT) == 0)) {
        SMP_TRACE_ERROR(
 "IO capability does not meet authentication requirement");
        smp_int_data.status = SMP_PAIR_AUTH_FAIL;
        int_evt = SMP_AUTH_CMPL_EVT;
 } else {
        p_cb->sec_level = SMP_SEC_UNAUTHENTICATE;
        SMP_TRACE_EVENT("p_cb->sec_level =%d (SMP_SEC_UNAUTHENTICATE) ",
                        p_cb->sec_level);

        tSMP_KEY key;
        key.key_type = SMP_KEY_TYPE_TK;
        key.p_data = p_cb->tk;
        smp_int_data.key = key;

        memset(p_cb->tk, 0, BT_OCTET16_LEN);
  
        int_evt = SMP_KEY_READY_EVT;
 }
 break;

 case SMP_MODEL_PASSKEY:
      p_cb->sec_level = SMP_SEC_AUTHENTICATED;
      SMP_TRACE_EVENT("p_cb->sec_level =%d (SMP_SEC_AUTHENTICATED) ",
                      p_cb->sec_level);

      p_cb->cb_evt = SMP_PASSKEY_REQ_EVT;
      int_evt = SMP_TK_REQ_EVT;
 break;

 case SMP_MODEL_OOB:
      SMP_TRACE_ERROR("Association Model = SMP_MODEL_OOB");
      p_cb->sec_level = SMP_SEC_AUTHENTICATED;
      SMP_TRACE_EVENT("p_cb->sec_level =%d (SMP_SEC_AUTHENTICATED) ",
                      p_cb->sec_level);

      p_cb->cb_evt = SMP_OOB_REQ_EVT;
      int_evt = SMP_TK_REQ_EVT;
 break;

 case SMP_MODEL_KEY_NOTIF:
      p_cb->sec_level = SMP_SEC_AUTHENTICATED;
      SMP_TRACE_DEBUG("Need to generate Passkey");

  
      smp_generate_passkey(p_cb, NULL);
 break;

 case SMP_MODEL_SEC_CONN_JUSTWORKS:
 case SMP_MODEL_SEC_CONN_NUM_COMP:
 case SMP_MODEL_SEC_CONN_PASSKEY_ENT:
 case SMP_MODEL_SEC_CONN_PASSKEY_DISP:
 case SMP_MODEL_SEC_CONN_OOB:
      int_evt = SMP_PUBL_KEY_EXCH_REQ_EVT;
 break;

 case SMP_MODEL_OUT_OF_RANGE:
      SMP_TRACE_ERROR("Association Model = SMP_MODEL_OUT_OF_RANGE (failed)");
      smp_int_data.status = SMP_UNKNOWN_IO_CAP;
      int_evt = SMP_AUTH_CMPL_EVT;
 break;

 default:
      SMP_TRACE_ERROR(
 "Association Model = %d (SOMETHING IS WRONG WITH THE CODE)",
          p_cb->selected_association_model);
      smp_int_data.status = SMP_UNKNOWN_IO_CAP;
      int_evt = SMP_AUTH_CMPL_EVT;
 }

  SMP_TRACE_EVENT("sec_level=%d ", p_cb->sec_level);
 if (int_evt) smp_sm_event(p_cb, int_evt, &smp_int_data);
}

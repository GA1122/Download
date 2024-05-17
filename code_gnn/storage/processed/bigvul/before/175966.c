void smp_start_secure_connection_phase1(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

 if (p_cb->selected_association_model == SMP_MODEL_SEC_CONN_JUSTWORKS) {
    p_cb->sec_level = SMP_SEC_UNAUTHENTICATE;
    SMP_TRACE_EVENT("p_cb->sec_level =%d (SMP_SEC_UNAUTHENTICATE) ",
                    p_cb->sec_level);
 } else {
    p_cb->sec_level = SMP_SEC_AUTHENTICATED;
    SMP_TRACE_EVENT("p_cb->sec_level =%d (SMP_SEC_AUTHENTICATED) ",
                    p_cb->sec_level);
 }

 switch (p_cb->selected_association_model) {
 case SMP_MODEL_SEC_CONN_JUSTWORKS:
 case SMP_MODEL_SEC_CONN_NUM_COMP:
      memset(p_cb->local_random, 0, BT_OCTET16_LEN);
      smp_start_nonce_generation(p_cb);
 break;
 case SMP_MODEL_SEC_CONN_PASSKEY_ENT:
  
      p_cb->cb_evt = SMP_PASSKEY_REQ_EVT;
      smp_sm_event(p_cb, SMP_TK_REQ_EVT, NULL);
 break;
 case SMP_MODEL_SEC_CONN_PASSKEY_DISP:
  
      SMP_TRACE_DEBUG("Need to generate SC Passkey");
      smp_generate_passkey(p_cb, NULL);
 break;
 case SMP_MODEL_SEC_CONN_OOB:
  
      smp_process_secure_connection_oob_data(p_cb, NULL);
 break;
 default:
      SMP_TRACE_ERROR("Association Model = %d is not used in LE SC",
                      p_cb->selected_association_model);
 break;
 }
}

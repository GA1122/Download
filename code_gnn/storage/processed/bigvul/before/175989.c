void smp_proc_pair_cmd(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = (uint8_t*)p_data;
 uint8_t reason = SMP_ENC_KEY_SIZE;
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_cb->pairing_bda);

  SMP_TRACE_DEBUG("%s", __func__);
  
 if (p_dev_rec && (p_cb->role == HCI_ROLE_SLAVE))
    btm_sec_clear_ble_keys(p_dev_rec);

  p_cb->flags |= SMP_PAIR_FLAG_ENC_AFTER_PAIR;

  STREAM_TO_UINT8(p_cb->peer_io_caps, p);
  STREAM_TO_UINT8(p_cb->peer_oob_flag, p);
  STREAM_TO_UINT8(p_cb->peer_auth_req, p);
  STREAM_TO_UINT8(p_cb->peer_enc_size, p);
  STREAM_TO_UINT8(p_cb->peer_i_key, p);
  STREAM_TO_UINT8(p_cb->peer_r_key, p);

 if (smp_command_has_invalid_parameters(p_cb)) {
    reason = SMP_INVALID_PARAMETERS;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

 if (pts_test_send_authentication_complete_failure(p_cb)) return;

 if (p_cb->role == HCI_ROLE_SLAVE) {
 if (!(p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD)) {
  
      p_cb->local_i_key = p_cb->peer_i_key;
      p_cb->local_r_key = p_cb->peer_r_key;

      p_cb->cb_evt = SMP_SEC_REQUEST_EVT;
 } else  
 {
  
      p_cb->local_i_key &= p_cb->peer_i_key;
      p_cb->local_r_key &= p_cb->peer_r_key;
      p_cb->selected_association_model = smp_select_association_model(p_cb);

 if (p_cb->secure_connections_only_mode_required &&
 (!(p_cb->le_secure_connections_mode_is_used) ||
 (p_cb->selected_association_model ==
            SMP_MODEL_SEC_CONN_JUSTWORKS))) {
        SMP_TRACE_ERROR(
 "%s: pairing failed - slave requires secure connection only mode",
            __func__);
        reason = SMP_PAIR_AUTH_FAIL;
        smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

 if (p_cb->selected_association_model == SMP_MODEL_SEC_CONN_OOB) {
 if (smp_request_oob_data(p_cb)) return;
 } else {
        smp_send_pair_rsp(p_cb, NULL);
 }
 }
 } else  
 {
    p_cb->selected_association_model = smp_select_association_model(p_cb);

 if (p_cb->secure_connections_only_mode_required &&
 (!(p_cb->le_secure_connections_mode_is_used) ||
 (p_cb->selected_association_model == SMP_MODEL_SEC_CONN_JUSTWORKS))) {
      SMP_TRACE_ERROR(
 "Master requires secure connection only mode "
 "but it can't be provided -> Master fails pairing");
      reason = SMP_PAIR_AUTH_FAIL;
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

 if (p_cb->selected_association_model == SMP_MODEL_SEC_CONN_OOB) {
 if (smp_request_oob_data(p_cb)) return;
 } else {
      smp_decide_association_model(p_cb, NULL);
 }
 }
}

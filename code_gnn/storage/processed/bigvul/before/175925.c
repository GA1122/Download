void smp_proc_compare(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
 if (!memcmp(p_cb->rconfirm, p_data->key.p_data, BT_OCTET16_LEN)) {
  
 if (p_cb->peer_enc_size < p_cb->loc_enc_size)
      p_cb->loc_enc_size = p_cb->peer_enc_size;

 if (p_cb->role == HCI_ROLE_SLAVE)
      smp_sm_event(p_cb, SMP_RAND_EVT, NULL);
 else {
  
      p_cb->local_i_key = p_cb->peer_i_key;
      p_cb->local_r_key = p_cb->peer_r_key;

      smp_sm_event(p_cb, SMP_ENC_REQ_EVT, NULL);
 }

 } else {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_CONFIRM_VALUE_ERR;
    p_cb->failure = SMP_CONFIRM_VALUE_ERR;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 }
}

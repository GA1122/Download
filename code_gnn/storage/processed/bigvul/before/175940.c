void smp_process_keypress_notification(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = p_data->p_data;

  SMP_TRACE_DEBUG("%s", __func__);
  p_cb->status = p_data->status;

 if (smp_command_has_invalid_parameters(p_cb)) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_INVALID_PARAMETERS;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 return;
 }

 if (p != NULL) {
    STREAM_TO_UINT8(p_cb->peer_keypress_notification, p);
 } else {
    p_cb->peer_keypress_notification = BTM_SP_KEY_OUT_OF_RANGE;
 }
  p_cb->cb_evt = SMP_PEER_KEYPR_NOT_EVT;
}

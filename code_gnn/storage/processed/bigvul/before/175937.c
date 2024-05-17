void smp_proc_sl_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t key_type = p_data->key.key_type;

  SMP_TRACE_DEBUG("%s", __func__);
 if (key_type == SMP_KEY_TYPE_TK) {
    smp_generate_srand_mrand_confirm(p_cb, NULL);
 } else if (key_type == SMP_KEY_TYPE_CFM) {
    smp_set_state(SMP_STATE_WAIT_CONFIRM);

 if (p_cb->flags & SMP_PAIR_FLAGS_CMD_CONFIRM)
      smp_sm_event(p_cb, SMP_CONFIRM_EVT, NULL);
 }
}

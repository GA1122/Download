void smp_process_local_nonce(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

 switch (p_cb->selected_association_model) {
 case SMP_MODEL_SEC_CONN_JUSTWORKS:
 case SMP_MODEL_SEC_CONN_NUM_COMP:
 if (p_cb->role == HCI_ROLE_SLAVE) {
  
        smp_calculate_local_commitment(p_cb);
        smp_send_commitment(p_cb, NULL);
  
        smp_set_state(SMP_STATE_WAIT_NONCE);
 } else  
 {
 if (p_cb->flags & SMP_PAIR_FLAG_HAVE_PEER_COMM) {
  
          SMP_TRACE_DEBUG(
 "master in assoc mode = %d "
 "already rcvd slave commitment - race condition",
              p_cb->selected_association_model);
          p_cb->flags &= ~SMP_PAIR_FLAG_HAVE_PEER_COMM;
          smp_send_rand(p_cb, NULL);
          smp_set_state(SMP_STATE_WAIT_NONCE);
 }
 }
 break;
 case SMP_MODEL_SEC_CONN_PASSKEY_ENT:
 case SMP_MODEL_SEC_CONN_PASSKEY_DISP:
      smp_calculate_local_commitment(p_cb);

 if (p_cb->role == HCI_ROLE_MASTER) {
        smp_send_commitment(p_cb, NULL);
 } else  
 {
 if (p_cb->flags & SMP_PAIR_FLAG_HAVE_PEER_COMM) {
  
          smp_send_commitment(p_cb, NULL);
          smp_set_state(SMP_STATE_WAIT_NONCE);
 }
 }
 break;
 case SMP_MODEL_SEC_CONN_OOB:
 if (p_cb->role == HCI_ROLE_MASTER) {
        smp_send_rand(p_cb, NULL);
 }

      smp_set_state(SMP_STATE_WAIT_NONCE);
 break;
 default:
      SMP_TRACE_ERROR("Association Model = %d is not used in LE SC",
                      p_cb->selected_association_model);
 break;
 }
}

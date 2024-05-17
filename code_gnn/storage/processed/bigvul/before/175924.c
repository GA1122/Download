void smp_pairing_cmpl(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 if (p_cb->total_tx_unacked == 0) {
  
    smp_proc_pairing_cmpl(p_cb);
 }
}

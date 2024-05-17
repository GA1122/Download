void smp_fast_conn_param(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  
  L2CA_EnableUpdateBleConnParams(p_cb->pairing_bda, false);
}

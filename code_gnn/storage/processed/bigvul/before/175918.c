void smp_key_distribution_by_transport(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
 if (p_cb->smp_over_br) {
    smp_br_select_next_key(p_cb, NULL);
 } else {
    smp_key_distribution(p_cb, NULL);
 }
}

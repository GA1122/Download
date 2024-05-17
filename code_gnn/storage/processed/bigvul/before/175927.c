void smp_proc_discard(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
 if (!(p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD))
    smp_reset_control_value(p_cb);
}

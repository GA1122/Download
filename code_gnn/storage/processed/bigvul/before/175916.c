void smp_idle_terminate(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 if (p_cb->flags & SMP_PAIR_FLAGS_WE_STARTED_DD) {
    SMP_TRACE_DEBUG("Pairing terminated at IDLE state.");
    p_cb->status = SMP_FAIL;
    smp_proc_pairing_cmpl(p_cb);
 }
}

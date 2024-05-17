void omx_vdec::perf_control::request_cores(int frame_duration_us)
{
 if (frame_duration_us > MIN_FRAME_DURATION_FOR_PERF_REQUEST_US) {
 return;
 }
    load_lib();
 if (m_perf_lock_acquire && m_perf_handle < 0) {
 int arg = 0x700   + 2  ;
        m_perf_handle = m_perf_lock_acquire(m_perf_handle, 0, &arg, sizeof(arg)/sizeof(int));
 if (m_perf_handle) {
            DEBUG_PRINT_HIGH("perf lock acquired");
 }
 }
}

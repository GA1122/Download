omx_vdec::perf_control::perf_control ()
{
    m_perf_lib = NULL;
    m_perf_handle = -1;
    m_perf_lock_acquire = NULL;
    m_perf_lock_release = NULL;
}

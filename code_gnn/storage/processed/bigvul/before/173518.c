omx_vdec::perf_control::~perf_control()
{
 if (m_perf_handle >= 0 && m_perf_lock_release) {
        DEBUG_PRINT_LOW("NOTE2: release perf lock");
        m_perf_lock_release(m_perf_handle);
 }
 if (m_perf_lib) {
        dlclose(m_perf_lib);
 }
}

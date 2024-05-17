omx_video::omx_video():
    c2d_opened(false),
    psource_frame(NULL),
    pdest_frame(NULL),
    secure_session(false),
    mEmptyEosBuffer(NULL),
    m_pipe_in(-1),
    m_pipe_out(-1),
    m_pInput_pmem(NULL),
    m_pOutput_pmem(NULL),
#ifdef USE_ION
    m_pInput_ion(NULL),
    m_pOutput_ion(NULL),
#endif
    m_error_propogated(false),
    m_state(OMX_StateInvalid),
    m_app_data(NULL),
    m_use_input_pmem(OMX_FALSE),
    m_use_output_pmem(OMX_FALSE),
    m_input_msg_id(OMX_COMPONENT_GENERATE_ETB),
    m_inp_mem_ptr(NULL),
    m_out_mem_ptr(NULL),
    input_flush_progress (false),
    output_flush_progress (false),
    input_use_buffer (false),
    output_use_buffer (false),
    pending_input_buffers(0),
    pending_output_buffers(0),
    m_out_bm_count(0),
    m_inp_bm_count(0),
    m_flags(0),
    m_etb_count(0),
    m_fbd_count(0),
    m_event_port_settings_sent(false),
    hw_overload(false)
{
    DEBUG_PRINT_HIGH("omx_video(): Inside Constructor()");
    memset(&m_cmp,0,sizeof(m_cmp));
    memset(&m_pCallbacks,0,sizeof(m_pCallbacks));
    async_thread_created = false;
    msg_thread_created = false;

    mUsesColorConversion = false;
    pthread_mutex_init(&m_lock, NULL);
    sem_init(&m_cmd_lock,0,0);
    DEBUG_PRINT_LOW("meta_buffer_hdr = %p", meta_buffer_hdr);
}

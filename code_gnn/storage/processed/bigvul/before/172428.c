venc_dev::venc_dev(class omx_venc *venc_class)
{
 int i = 0;
    venc_handle = venc_class;
    etb = ebd = ftb = fbd = 0;

 for (i = 0; i < MAX_PORT; i++)
        streaming[i] = false;

    stopped = 1;
    paused = false;
    async_thread_created = false;
    color_format = 0;
    hw_overload = false;
    pthread_mutex_init(&pause_resume_mlock, NULL);
    pthread_cond_init(&pause_resume_cond, NULL);
    memset(&extradata_info, 0, sizeof(extradata_info));
    memset(&idrperiod, 0, sizeof(idrperiod));
    memset(&multislice, 0, sizeof(multislice));
    memset (&slice_mode, 0 , sizeof(slice_mode));
    memset(&m_sVenc_cfg, 0, sizeof(m_sVenc_cfg));
    memset(&rate_ctrl, 0, sizeof(rate_ctrl));
    memset(&bitrate, 0, sizeof(bitrate));
    memset(&intra_period, 0, sizeof(intra_period));
    memset(&codec_profile, 0, sizeof(codec_profile));
    memset(&set_param, 0, sizeof(set_param));
    memset(&time_inc, 0, sizeof(time_inc));
    memset(&m_sInput_buff_property, 0, sizeof(m_sInput_buff_property));
    memset(&m_sOutput_buff_property, 0, sizeof(m_sOutput_buff_property));
    memset(&session_qp, 0, sizeof(session_qp));
    memset(&entropy, 0, sizeof(entropy));
    memset(&dbkfilter, 0, sizeof(dbkfilter));
    memset(&intra_refresh, 0, sizeof(intra_refresh));
    memset(&hec, 0, sizeof(hec));
    memset(&voptimecfg, 0, sizeof(voptimecfg));
    memset(&capability, 0, sizeof(capability));
    memset(&m_debug,0,sizeof(m_debug));
    memset(&hier_layers,0,sizeof(hier_layers));
    is_searchrange_set = false;
    enable_mv_narrow_searchrange = false;
    supported_rc_modes = RC_ALL;
    camera_mode_enabled = false;
    memset(&ltrinfo, 0, sizeof(ltrinfo));
    sess_priority.priority = 1;
    operating_rate = 0;

 char property_value[PROPERTY_VALUE_MAX] = {0};
    property_get("vidc.enc.log.in", property_value, "0");
    m_debug.in_buffer_log = atoi(property_value);

    property_get("vidc.enc.log.out", property_value, "0");
    m_debug.out_buffer_log = atoi(property_value);

    property_get("vidc.enc.log.extradata", property_value, "0");
    m_debug.extradata_log = atoi(property_value);

    snprintf(m_debug.log_loc, PROPERTY_VALUE_MAX,
 "%s", BUFFER_LOG_LOC);
}

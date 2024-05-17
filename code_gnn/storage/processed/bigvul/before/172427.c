void venc_dev::venc_config_print()
{

    DEBUG_PRINT_HIGH("ENC_CONFIG: Codec: %ld, Profile %ld, level : %ld",
            m_sVenc_cfg.codectype, codec_profile.profile, profile_level.level);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Input Width: %ld, Height:%ld, Fps: %ld",
            m_sVenc_cfg.input_width, m_sVenc_cfg.input_height,
            m_sVenc_cfg.fps_num/m_sVenc_cfg.fps_den);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Output Width: %ld, Height:%ld, Fps: %ld",
            m_sVenc_cfg.dvs_width, m_sVenc_cfg.dvs_height,
            m_sVenc_cfg.fps_num/m_sVenc_cfg.fps_den);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Bitrate: %ld, RC: %ld, P - Frames : %ld, B - Frames = %ld",
            bitrate.target_bitrate, rate_ctrl.rcmode, intra_period.num_pframes, intra_period.num_bframes);

    DEBUG_PRINT_HIGH("ENC_CONFIG: qpI: %ld, qpP: %ld, qpb: %ld",
            session_qp.iframeqp, session_qp.pframeqp, session_qp.bframeqp);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Init_qpI: %ld, Init_qpP: %ld, Init_qpb: %ld",
            init_qp.iframeqp, init_qp.pframeqp, init_qp.bframeqp);

    DEBUG_PRINT_HIGH("ENC_CONFIG: minQP: %lu, maxQP: %lu",
            session_qp_values.minqp, session_qp_values.maxqp);

    DEBUG_PRINT_HIGH("ENC_CONFIG: VOP_Resolution: %ld, Slice-Mode: %ld, Slize_Size: %ld",
            voptimecfg.voptime_resolution, multislice.mslice_mode,
            multislice.mslice_size);

    DEBUG_PRINT_HIGH("ENC_CONFIG: EntropyMode: %d, CabacModel: %ld",
            entropy.longentropysel, entropy.cabacmodel);

    DEBUG_PRINT_HIGH("ENC_CONFIG: DB-Mode: %ld, alpha: %ld, Beta: %ld",
            dbkfilter.db_mode, dbkfilter.slicealpha_offset,
            dbkfilter.slicebeta_offset);

    DEBUG_PRINT_HIGH("ENC_CONFIG: IntraMB/Frame: %ld, HEC: %ld, IDR Period: %ld",
            intra_refresh.mbcount, hec.header_extension, idrperiod.idrperiod);

    DEBUG_PRINT_HIGH("ENC_CONFIG: LTR Enabled: %d, Count: %d",
            ltrinfo.enabled, ltrinfo.count);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Hier layers: %d, Hier Mode: %s VPX_ErrorResilience: %d",
            hier_layers.numlayers, hiermode_string(hier_layers.hier_mode), vpx_err_resilience.enable);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Performace level: %d", performance_level.perflevel);

    DEBUG_PRINT_HIGH("ENC_CONFIG: VUI timing info enabled: %d", vui_timing_info.enabled);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Peak bitrate: %d", peak_bitrate.peakbitrate);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Session Priority: %u", sess_priority.priority);

    DEBUG_PRINT_HIGH("ENC_CONFIG: Operating Rate: %u", operating_rate);
}

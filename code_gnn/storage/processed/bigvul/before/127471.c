    JPEGImageReader(JPEGImageDecoder* decoder)
        : m_decoder(decoder)
        , m_bufferLength(0)
        , m_bytesToSkip(0)
        , m_state(JPEG_HEADER)
        , m_samples(0)
#if USE(QCMSLIB)
        , m_transform(0)
#endif
    {
        memset(&m_info, 0, sizeof(jpeg_decompress_struct));

        m_info.err = jpeg_std_error(&m_err.pub);
        m_err.pub.error_exit = error_exit;

        jpeg_create_decompress(&m_info);

        decoder_source_mgr* src = 0;
        if (!m_info.src) {
            src = (decoder_source_mgr*)fastCalloc(sizeof(decoder_source_mgr), 1);
            if (!src) {
                m_state = JPEG_ERROR;
                return;
            }
        }

        m_info.src = (jpeg_source_mgr*)src;

        src->pub.init_source = init_source;
        src->pub.fill_input_buffer = fill_input_buffer;
        src->pub.skip_input_data = skip_input_data;
        src->pub.resync_to_restart = jpeg_resync_to_restart;
        src->pub.term_source = term_source;
        src->decoder = this;

#if USE(ICCJPEG)
        setup_read_icc_profile(&m_info);
#endif

        jpeg_save_markers(&m_info, exifMarker, 0xFFFF);
    }

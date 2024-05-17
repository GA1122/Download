void StreamTcpInitConfig(char quiet)
{
    intmax_t value = 0;
    uint16_t rdrange = 10;

    SCLogDebug("Initializing Stream");

    memset(&stream_config,  0, sizeof(stream_config));

    SC_ATOMIC_INIT(stream_config.memcap);
    SC_ATOMIC_INIT(stream_config.reassembly_memcap);

    if ((ConfGetInt("stream.max-sessions", &value)) == 1) {
        SCLogWarning(SC_WARN_OPTION_OBSOLETE, "max-sessions is obsolete. "
            "Number of concurrent sessions is now only limited by Flow and "
            "TCP stream engine memcaps.");
    }

    if ((ConfGetInt("stream.prealloc-sessions", &value)) == 1) {
        stream_config.prealloc_sessions = (uint32_t)value;
    } else {
        if (RunmodeIsUnittests()) {
            stream_config.prealloc_sessions = 128;
        } else {
            stream_config.prealloc_sessions = STREAMTCP_DEFAULT_PREALLOC;
            if (ConfGetNode("stream.prealloc-sessions") != NULL) {
                WarnInvalidConfEntry("stream.prealloc_sessions",
                                     "%"PRIu32,
                                     stream_config.prealloc_sessions);
            }
        }
    }
    if (!quiet) {
        SCLogConfig("stream \"prealloc-sessions\": %"PRIu32" (per thread)",
                stream_config.prealloc_sessions);
    }

    const char *temp_stream_memcap_str;
    if (ConfGetValue("stream.memcap", &temp_stream_memcap_str) == 1) {
        uint64_t stream_memcap_copy;
        if (ParseSizeStringU64(temp_stream_memcap_str, &stream_memcap_copy) < 0) {
            SCLogError(SC_ERR_SIZE_PARSE, "Error parsing stream.memcap "
                       "from conf file - %s.  Killing engine",
                       temp_stream_memcap_str);
            exit(EXIT_FAILURE);
        } else {
            SC_ATOMIC_SET(stream_config.memcap, stream_memcap_copy);
        }
    } else {
        SC_ATOMIC_SET(stream_config.memcap, STREAMTCP_DEFAULT_MEMCAP);
    }

    if (!quiet) {
        SCLogConfig("stream \"memcap\": %"PRIu64, SC_ATOMIC_GET(stream_config.memcap));
    }

    ConfGetBool("stream.midstream", &stream_config.midstream);

    if (!quiet) {
        SCLogConfig("stream \"midstream\" session pickups: %s", stream_config.midstream ? "enabled" : "disabled");
    }

    ConfGetBool("stream.async-oneside", &stream_config.async_oneside);

    if (!quiet) {
        SCLogConfig("stream \"async-oneside\": %s", stream_config.async_oneside ? "enabled" : "disabled");
    }

    int csum = 0;

    if ((ConfGetBool("stream.checksum-validation", &csum)) == 1) {
        if (csum == 1) {
            stream_config.flags |= STREAMTCP_INIT_FLAG_CHECKSUM_VALIDATION;
        }
     
    } else {
        stream_config.flags |= STREAMTCP_INIT_FLAG_CHECKSUM_VALIDATION;
    }

    if (!quiet) {
        SCLogConfig("stream \"checksum-validation\": %s",
                stream_config.flags & STREAMTCP_INIT_FLAG_CHECKSUM_VALIDATION ?
                "enabled" : "disabled");
    }

    const char *temp_stream_inline_str;
    if (ConfGetValue("stream.inline", &temp_stream_inline_str) == 1) {
        int inl = 0;

         
        if (strcmp(temp_stream_inline_str, "auto") == 0) {
            if (EngineModeIsIPS()) {
                stream_config.flags |= STREAMTCP_INIT_FLAG_INLINE;
            }
        } else if (ConfGetBool("stream.inline", &inl) == 1) {
            if (inl) {
                stream_config.flags |= STREAMTCP_INIT_FLAG_INLINE;
            }
        }
    } else {
         
        if (EngineModeIsIPS()) {
            stream_config.flags |= STREAMTCP_INIT_FLAG_INLINE;
        }
    }

    if (!quiet) {
        SCLogConfig("stream.\"inline\": %s",
                    stream_config.flags & STREAMTCP_INIT_FLAG_INLINE
                    ? "enabled" : "disabled");
    }

    int bypass = 0;
    if ((ConfGetBool("stream.bypass", &bypass)) == 1) {
        if (bypass == 1) {
            stream_config.flags |= STREAMTCP_INIT_FLAG_BYPASS;
        }
    }

    if (!quiet) {
        SCLogConfig("stream \"bypass\": %s",
                    (stream_config.flags & STREAMTCP_INIT_FLAG_BYPASS)
                    ? "enabled" : "disabled");
    }

    int drop_invalid = 0;
    if ((ConfGetBool("stream.drop-invalid", &drop_invalid)) == 1) {
        if (drop_invalid == 1) {
            stream_config.flags |= STREAMTCP_INIT_FLAG_DROP_INVALID;
        }
    } else {
        stream_config.flags |= STREAMTCP_INIT_FLAG_DROP_INVALID;
    }

    if ((ConfGetInt("stream.max-synack-queued", &value)) == 1) {
        if (value >= 0 && value <= 255) {
            stream_config.max_synack_queued = (uint8_t)value;
        } else {
            stream_config.max_synack_queued = (uint8_t)STREAMTCP_DEFAULT_MAX_SYNACK_QUEUED;
        }
    } else {
        stream_config.max_synack_queued = (uint8_t)STREAMTCP_DEFAULT_MAX_SYNACK_QUEUED;
    }
    if (!quiet) {
        SCLogConfig("stream \"max-synack-queued\": %"PRIu8, stream_config.max_synack_queued);
    }

    const char *temp_stream_reassembly_memcap_str;
    if (ConfGetValue("stream.reassembly.memcap", &temp_stream_reassembly_memcap_str) == 1) {
        uint64_t stream_reassembly_memcap_copy;
        if (ParseSizeStringU64(temp_stream_reassembly_memcap_str,
                               &stream_reassembly_memcap_copy) < 0) {
            SCLogError(SC_ERR_SIZE_PARSE, "Error parsing "
                       "stream.reassembly.memcap "
                       "from conf file - %s.  Killing engine",
                       temp_stream_reassembly_memcap_str);
            exit(EXIT_FAILURE);
        } else {
            SC_ATOMIC_SET(stream_config.reassembly_memcap, stream_reassembly_memcap_copy);
        }
    } else {
        SC_ATOMIC_SET(stream_config.reassembly_memcap , STREAMTCP_DEFAULT_REASSEMBLY_MEMCAP);
    }

    if (!quiet) {
        SCLogConfig("stream.reassembly \"memcap\": %"PRIu64"",
                    SC_ATOMIC_GET(stream_config.reassembly_memcap));
    }

    const char *temp_stream_reassembly_depth_str;
    if (ConfGetValue("stream.reassembly.depth", &temp_stream_reassembly_depth_str) == 1) {
        if (ParseSizeStringU32(temp_stream_reassembly_depth_str,
                               &stream_config.reassembly_depth) < 0) {
            SCLogError(SC_ERR_SIZE_PARSE, "Error parsing "
                       "stream.reassembly.depth "
                       "from conf file - %s.  Killing engine",
                       temp_stream_reassembly_depth_str);
            exit(EXIT_FAILURE);
        }
    } else {
        stream_config.reassembly_depth = 0;
    }

    if (!quiet) {
        SCLogConfig("stream.reassembly \"depth\": %"PRIu32"", stream_config.reassembly_depth);
    }

    int randomize = 0;
    if ((ConfGetBool("stream.reassembly.randomize-chunk-size", &randomize)) == 0) {
         
        if (!(RunmodeIsUnittests()))
            randomize = 1;
    }

    if (randomize) {
        const char *temp_rdrange;
        if (ConfGetValue("stream.reassembly.randomize-chunk-range",
                    &temp_rdrange) == 1) {
            if (ParseSizeStringU16(temp_rdrange, &rdrange) < 0) {
                SCLogError(SC_ERR_SIZE_PARSE, "Error parsing "
                        "stream.reassembly.randomize-chunk-range "
                        "from conf file - %s.  Killing engine",
                        temp_rdrange);
                exit(EXIT_FAILURE);
            } else if (rdrange >= 100) {
                SCLogError(SC_ERR_INVALID_VALUE,
                           "stream.reassembly.randomize-chunk-range "
                           "must be lower than 100");
                exit(EXIT_FAILURE);
            }
        }
    }

    const char *temp_stream_reassembly_toserver_chunk_size_str;
    if (ConfGetValue("stream.reassembly.toserver-chunk-size",
                &temp_stream_reassembly_toserver_chunk_size_str) == 1) {
        if (ParseSizeStringU16(temp_stream_reassembly_toserver_chunk_size_str,
                               &stream_config.reassembly_toserver_chunk_size) < 0) {
            SCLogError(SC_ERR_SIZE_PARSE, "Error parsing "
                       "stream.reassembly.toserver-chunk-size "
                       "from conf file - %s.  Killing engine",
                       temp_stream_reassembly_toserver_chunk_size_str);
            exit(EXIT_FAILURE);
        }
    } else {
        stream_config.reassembly_toserver_chunk_size =
            STREAMTCP_DEFAULT_TOSERVER_CHUNK_SIZE;
    }

    if (randomize) {
        long int r = RandomGetWrap();
        stream_config.reassembly_toserver_chunk_size +=
            (int) (stream_config.reassembly_toserver_chunk_size *
                   (r * 1.0 / RAND_MAX - 0.5) * rdrange / 100);
    }
    const char *temp_stream_reassembly_toclient_chunk_size_str;
    if (ConfGetValue("stream.reassembly.toclient-chunk-size",
                &temp_stream_reassembly_toclient_chunk_size_str) == 1) {
        if (ParseSizeStringU16(temp_stream_reassembly_toclient_chunk_size_str,
                               &stream_config.reassembly_toclient_chunk_size) < 0) {
            SCLogError(SC_ERR_SIZE_PARSE, "Error parsing "
                       "stream.reassembly.toclient-chunk-size "
                       "from conf file - %s.  Killing engine",
                       temp_stream_reassembly_toclient_chunk_size_str);
            exit(EXIT_FAILURE);
        }
    } else {
        stream_config.reassembly_toclient_chunk_size =
            STREAMTCP_DEFAULT_TOCLIENT_CHUNK_SIZE;
    }

    if (randomize) {
        long int r = RandomGetWrap();
        stream_config.reassembly_toclient_chunk_size +=
            (int) (stream_config.reassembly_toclient_chunk_size *
                   (r * 1.0 / RAND_MAX - 0.5) * rdrange / 100);
    }
    if (!quiet) {
        SCLogConfig("stream.reassembly \"toserver-chunk-size\": %"PRIu16,
            stream_config.reassembly_toserver_chunk_size);
        SCLogConfig("stream.reassembly \"toclient-chunk-size\": %"PRIu16,
            stream_config.reassembly_toclient_chunk_size);
    }

    int enable_raw = 1;
    if (ConfGetBool("stream.reassembly.raw", &enable_raw) == 1) {
        if (!enable_raw) {
            stream_config.stream_init_flags = STREAMTCP_STREAM_FLAG_DISABLE_RAW;
        }
    } else {
        enable_raw = 1;
    }
    if (!quiet)
        SCLogConfig("stream.reassembly.raw: %s", enable_raw ? "enabled" : "disabled");

     
    StreamTcpInitMemuse();
    StatsRegisterGlobalCounter("tcp.memuse", StreamTcpMemuseCounter);

    StreamTcpReassembleInit(quiet);

     
    FlowSetProtoFreeFunc(IPPROTO_TCP, StreamTcpSessionClear);

#ifdef UNITTESTS
    if (RunmodeIsUnittests()) {
        SCMutexLock(&ssn_pool_mutex);
        if (ssn_pool == NULL) {
            ssn_pool = PoolThreadInit(1,  
                    0,  
                    stream_config.prealloc_sessions,
                    sizeof(TcpSession),
                    StreamTcpSessionPoolAlloc,
                    StreamTcpSessionPoolInit, NULL,
                    StreamTcpSessionPoolCleanup, NULL);
        }
        SCMutexUnlock(&ssn_pool_mutex);
    }
#endif
}
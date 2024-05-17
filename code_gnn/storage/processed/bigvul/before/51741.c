proto_register_ppi(void)
{
    static hf_register_info hf[] = {
    { &hf_ppi_head_version,
      { "Version", "ppi.version",
        FT_UINT8, BASE_DEC, NULL, 0x0,
        "PPI header format version", HFILL } },
    { &hf_ppi_head_flags,
      { "Flags", "ppi.flags",
        FT_UINT8, BASE_HEX, NULL, 0x0,
        "PPI header flags", HFILL } },
    { &hf_ppi_head_flag_alignment,
      { "Alignment", "ppi.flags.alignment",
        FT_BOOLEAN, 8, TFS(&tfs_ppi_head_flag_alignment), 0x01,
        "PPI header flags - 32bit Alignment", HFILL } },
    { &hf_ppi_head_flag_reserved,
      { "Reserved", "ppi.flags.reserved",
        FT_UINT8, BASE_HEX, NULL, 0xFE,
        "PPI header flags - Reserved Flags", HFILL } },
    { &hf_ppi_head_len,
       { "Header length", "ppi.length",
         FT_UINT16, BASE_DEC, NULL, 0x0,
         "Length of header including payload", HFILL } },
    { &hf_ppi_head_dlt,
       { "DLT", "ppi.dlt",
         FT_UINT32, BASE_DEC, NULL, 0x0, "libpcap Data Link Type (DLT) of the payload", HFILL } },

    { &hf_ppi_field_type,
       { "Field type", "ppi.field_type",
         FT_UINT16, BASE_DEC, VALS(vs_ppi_field_type), 0x0, "PPI data field type", HFILL } },
    { &hf_ppi_field_len,
       { "Field length", "ppi.field_len",
         FT_UINT16, BASE_DEC, NULL, 0x0, "PPI data field length", HFILL } },

    { &hf_80211_common_tsft,
       { "TSFT", "ppi.80211-common.tsft",
         FT_UINT64, BASE_DEC, NULL, 0x0, "PPI 802.11-Common Timing Synchronization Function Timer (TSFT)", HFILL } },
    { &hf_80211_common_flags,
       { "Flags", "ppi.80211-common.flags",
         FT_UINT16, BASE_HEX, NULL, 0x0, "PPI 802.11-Common Flags", HFILL } },
    { &hf_80211_common_flags_fcs,
       { "FCS present flag", "ppi.80211-common.flags.fcs",
         FT_BOOLEAN, 16, TFS(&tfs_present_absent), DOT11_FLAG_HAVE_FCS, "PPI 802.11-Common Frame Check Sequence (FCS) Present Flag", HFILL } },
    { &hf_80211_common_flags_tsft,
       { "TSFT flag", "ppi.80211-common.flags.tsft",
         FT_BOOLEAN, 16, TFS(&tfs_tsft_ms), DOT11_FLAG_TSF_TIMER_MS, "PPI 802.11-Common Timing Synchronization Function Timer (TSFT) msec/usec flag", HFILL } },
    { &hf_80211_common_flags_fcs_valid,
       { "FCS validity", "ppi.80211-common.flags.fcs-invalid",
         FT_BOOLEAN, 16, TFS(&tfs_invalid_valid), DOT11_FLAG_FCS_INVALID, "PPI 802.11-Common Frame Check Sequence (FCS) Validity flag", HFILL } },
    { &hf_80211_common_flags_phy_err,
       { "PHY error flag", "ppi.80211-common.flags.phy-err",
         FT_BOOLEAN, 16, TFS(&tfs_phy_error), DOT11_FLAG_PHY_ERROR, "PPI 802.11-Common Physical level (PHY) Error", HFILL } },
    { &hf_80211_common_rate,
       { "Data rate", "ppi.80211-common.rate",
         FT_UINT16, BASE_DEC, NULL, 0x0, "PPI 802.11-Common Data Rate (x 500 Kbps)", HFILL } },
    { &hf_80211_common_chan_freq,
       { "Channel frequency", "ppi.80211-common.chan.freq",
         FT_UINT16, BASE_DEC, NULL, 0x0,
        "PPI 802.11-Common Channel Frequency", HFILL } },
    { &hf_80211_common_chan_flags,
       { "Channel flags", "ppi.80211-common.chan.flags",
         FT_UINT16, BASE_HEX, NULL, 0x0, "PPI 802.11-Common Channel Flags", HFILL } },

    { &hf_80211_common_chan_flags_turbo,
       { "Turbo", "ppi.80211-common.chan.flags.turbo",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_TURBO, "PPI 802.11-Common Channel Flags Turbo", HFILL } },
    { &hf_80211_common_chan_flags_cck,
       { "Complementary Code Keying (CCK)", "ppi.80211-common.chan.flags.cck",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_CCK, "PPI 802.11-Common Channel Flags Complementary Code Keying (CCK) Modulation", HFILL } },
    { &hf_80211_common_chan_flags_ofdm,
       { "Orthogonal Frequency-Division Multiplexing (OFDM)", "ppi.80211-common.chan.flags.ofdm",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_OFDM, "PPI 802.11-Common Channel Flags Orthogonal Frequency-Division Multiplexing (OFDM)", HFILL } },
    { &hf_80211_common_chan_flags_2ghz,
       { "2 GHz spectrum", "ppi.80211-common.chan.flags.2ghz",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_2GHZ, "PPI 802.11-Common Channel Flags 2 GHz spectrum", HFILL } },
    { &hf_80211_common_chan_flags_5ghz,
       { "5 GHz spectrum", "ppi.80211-common.chan.flags.5ghz",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_5GHZ, "PPI 802.11-Common Channel Flags 5 GHz spectrum", HFILL } },
    { &hf_80211_common_chan_flags_passive,
       { "Passive", "ppi.80211-common.chan.flags.passive",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_PASSIVE, "PPI 802.11-Common Channel Flags Passive", HFILL } },
    { &hf_80211_common_chan_flags_dynamic,
       { "Dynamic CCK-OFDM", "ppi.80211-common.chan.flags.dynamic",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_DYN, "PPI 802.11-Common Channel Flags Dynamic CCK-OFDM Channel", HFILL } },
    { &hf_80211_common_chan_flags_gfsk,
       { "Gaussian Frequency Shift Keying (GFSK)", "ppi.80211-common.chan.flags.gfsk",
         FT_BOOLEAN, 16, NULL, IEEE80211_CHAN_GFSK, "PPI 802.11-Common Channel Flags Gaussian Frequency Shift Keying (GFSK) Modulation", HFILL } },

    { &hf_80211_common_fhss_hopset,
       { "FHSS hopset", "ppi.80211-common.fhss.hopset",
         FT_UINT8, BASE_HEX, NULL, 0x0, "PPI 802.11-Common Frequency-Hopping Spread Spectrum (FHSS) Hopset", HFILL } },
    { &hf_80211_common_fhss_pattern,
       { "FHSS pattern", "ppi.80211-common.fhss.pattern",
         FT_UINT8, BASE_HEX, NULL, 0x0, "PPI 802.11-Common Frequency-Hopping Spread Spectrum (FHSS) Pattern", HFILL } },
    { &hf_80211_common_dbm_antsignal,
       { "dBm antenna signal", "ppi.80211-common.dbm.antsignal",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11-Common dBm Antenna Signal", HFILL } },
    { &hf_80211_common_dbm_antnoise,
       { "dBm antenna noise", "ppi.80211-common.dbm.antnoise",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11-Common dBm Antenna Noise", HFILL } },

     
    { &hf_80211n_mac_flags,
       { "MAC flags", "ppi.80211n-mac.flags",
         FT_UINT32, BASE_HEX, NULL, 0x0, "PPI 802.11n MAC flags", HFILL } },
    { &hf_80211n_mac_flags_greenfield,
       { "Greenfield flag", "ppi.80211n-mac.flags.greenfield",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_GREENFIELD, "PPI 802.11n MAC Greenfield Flag", HFILL } },
    { &hf_80211n_mac_flags_ht20_40,
       { "HT20/HT40 flag", "ppi.80211n-mac.flags.ht20_40",
         FT_BOOLEAN, 32, TFS(&tfs_ht20_40), DOT11N_FLAG_HT40, "PPI 802.11n MAC HT20/HT40 Flag", HFILL } },
    { &hf_80211n_mac_flags_rx_guard_interval,
       { "RX Short Guard Interval (SGI) flag", "ppi.80211n-mac.flags.rx.short_guard_interval",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_SHORT_GI, "PPI 802.11n MAC RX Short Guard Interval (SGI) Flag", HFILL } },
    { &hf_80211n_mac_flags_duplicate_rx,
       { "Duplicate RX flag", "ppi.80211n-mac.flags.rx.duplicate",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_DUPLICATE_RX, "PPI 802.11n MAC Duplicate RX Flag", HFILL } },
    { &hf_80211n_mac_flags_aggregate,
       { "Aggregate flag", "ppi.80211n-mac.flags.agg",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_IS_AGGREGATE, "PPI 802.11 MAC Aggregate Flag", HFILL } },
    { &hf_80211n_mac_flags_more_aggregates,
       { "More aggregates flag", "ppi.80211n-mac.flags.more_agg",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_MORE_AGGREGATES, "PPI 802.11n MAC More Aggregates Flag", HFILL } },
    { &hf_80211n_mac_flags_delimiter_crc_after,
       { "A-MPDU Delimiter CRC error after this frame flag", "ppi.80211n-mac.flags.delim_crc_error_after",
         FT_BOOLEAN, 32, TFS(&tfs_true_false), DOT11N_FLAG_AGG_CRC_ERROR, "PPI 802.11n MAC A-MPDU Delimiter CRC Error After This Frame Flag", HFILL } },
    { &hf_80211n_mac_ampdu_id,
       { "AMPDU-ID", "ppi.80211n-mac.ampdu_id",
         FT_UINT32, BASE_HEX, NULL, 0x0, "PPI 802.11n MAC AMPDU-ID", HFILL } },
    { &hf_80211n_mac_num_delimiters,
       { "Num-Delimiters", "ppi.80211n-mac.num_delimiters",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC number of zero-length pad delimiters", HFILL } },
    { &hf_80211n_mac_reserved,
       { "Reserved", "ppi.80211n-mac.reserved",
         FT_UINT24, BASE_HEX, NULL, 0x0, "PPI 802.11n MAC Reserved", HFILL } },


     
    { &hf_80211n_mac_phy_mcs,
       { "MCS", "ppi.80211n-mac-phy.mcs",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Modulation Coding Scheme (MCS)", HFILL } },
    { &hf_80211n_mac_phy_num_streams,
       { "Number of spatial streams", "ppi.80211n-mac-phy.num_streams",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY number of spatial streams", HFILL } },
    { &hf_80211n_mac_phy_rssi_combined,
       { "RSSI combined", "ppi.80211n-mac-phy.rssi.combined",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Received Signal Strength Indication (RSSI) Combined", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant0_ctl,
       { "Antenna 0 control RSSI", "ppi.80211n-mac-phy.rssi.ant0ctl",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 0 Control Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant1_ctl,
       { "Antenna 1 control RSSI", "ppi.80211n-mac-phy.rssi.ant1ctl",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 1 Control Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant2_ctl,
       { "Antenna 2 control RSSI", "ppi.80211n-mac-phy.rssi.ant2ctl",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 2 Control Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant3_ctl,
       { "Antenna 3 control RSSI", "ppi.80211n-mac-phy.rssi.ant3ctl",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 3 Control Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant0_ext,
       { "Antenna 0 extension RSSI", "ppi.80211n-mac-phy.rssi.ant0ext",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 0 Extension Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant1_ext,
       { "Antenna 1 extension RSSI", "ppi.80211n-mac-phy.rssi.ant1ext",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 1 Extension Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant2_ext,
       { "Antenna 2 extension RSSI", "ppi.80211n-mac-phy.rssi.ant2ext",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 2 Extension Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_rssi_ant3_ext,
       { "Antenna 3 extension RSSI", "ppi.80211n-mac-phy.rssi.ant3ext",
         FT_UINT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Antenna 3 Extension Channel Received Signal Strength Indication (RSSI)", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_freq,
       { "Extended channel frequency", "ppi.80211-mac-phy.ext-chan.freq",
         FT_UINT16, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Extended Channel Frequency", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags,
       { "Channel flags", "ppi.80211-mac-phy.ext-chan.flags",
         FT_UINT16, BASE_HEX, NULL, 0x0, "PPI 802.11n MAC+PHY Channel Flags", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_turbo,
       { "Turbo", "ppi.80211-mac-phy.ext-chan.flags.turbo",
         FT_BOOLEAN, 16, NULL, 0x0010, "PPI 802.11n MAC+PHY Channel Flags Turbo", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_cck,
       { "Complementary Code Keying (CCK)", "ppi.80211-mac-phy.ext-chan.flags.cck",
         FT_BOOLEAN, 16, NULL, 0x0020, "PPI 802.11n MAC+PHY Channel Flags Complementary Code Keying (CCK) Modulation", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_ofdm,
       { "Orthogonal Frequency-Division Multiplexing (OFDM)", "ppi.80211-mac-phy.ext-chan.flags.ofdm",
         FT_BOOLEAN, 16, NULL, 0x0040, "PPI 802.11n MAC+PHY Channel Flags Orthogonal Frequency-Division Multiplexing (OFDM)", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_2ghz,
       { "2 GHz spectrum", "ppi.80211-mac-phy.ext-chan.flags.2ghz",
         FT_BOOLEAN, 16, NULL, 0x0080, "PPI 802.11n MAC+PHY Channel Flags 2 GHz spectrum", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_5ghz,
       { "5 GHz spectrum", "ppi.80211-mac-phy.ext-chan.flags.5ghz",
         FT_BOOLEAN, 16, NULL, 0x0100, "PPI 802.11n MAC+PHY Channel Flags 5 GHz spectrum", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_passive,
       { "Passive", "ppi.80211-mac-phy.ext-chan.flags.passive",
         FT_BOOLEAN, 16, NULL, 0x0200, "PPI 802.11n MAC+PHY Channel Flags Passive", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_dynamic,
       { "Dynamic CCK-OFDM", "ppi.80211-mac-phy.ext-chan.flags.dynamic",
         FT_BOOLEAN, 16, NULL, 0x0400, "PPI 802.11n MAC+PHY Channel Flags Dynamic CCK-OFDM Channel", HFILL } },
    { &hf_80211n_mac_phy_ext_chan_flags_gfsk,
       { "Gaussian Frequency Shift Keying (GFSK)", "ppi.80211-mac-phy.ext-chan.flags.gfsk",
         FT_BOOLEAN, 16, NULL, 0x0800, "PPI 802.11n MAC+PHY Channel Flags Gaussian Frequency Shift Keying (GFSK) Modulation", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant0signal,
       { "dBm antenna 0 signal", "ppi.80211n-mac-phy.dbmant0.signal",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 0 Signal", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant0noise,
       { "dBm antenna 0 noise", "ppi.80211n-mac-phy.dbmant0.noise",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 0 Noise", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant1signal,
       { "dBm antenna 1 signal", "ppi.80211n-mac-phy.dbmant1.signal",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 1 Signal", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant1noise,
       { "dBm antenna 1 noise", "ppi.80211n-mac-phy.dbmant1.noise",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 1 Noise", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant2signal,
       { "dBm antenna 2 signal", "ppi.80211n-mac-phy.dbmant2.signal",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 2 Signal", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant2noise,
       { "dBm antenna 2 noise", "ppi.80211n-mac-phy.dbmant2.noise",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 2 Noise", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant3signal,
       { "dBm antenna 3 signal", "ppi.80211n-mac-phy.dbmant3.signal",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 3 Signal", HFILL } },
    { &hf_80211n_mac_phy_dbm_ant3noise,
       { "dBm antenna 3 noise", "ppi.80211n-mac-phy.dbmant3.noise",
         FT_INT8, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY dBm Antenna 3 Noise", HFILL } },
    { &hf_80211n_mac_phy_evm0,
       { "EVM-0", "ppi.80211n-mac-phy.evm0",
         FT_UINT32, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Error Vector Magnitude (EVM) for chain 0", HFILL } },
    { &hf_80211n_mac_phy_evm1,
       { "EVM-1", "ppi.80211n-mac-phy.evm1",
         FT_UINT32, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Error Vector Magnitude (EVM) for chain 1", HFILL } },
    { &hf_80211n_mac_phy_evm2,
       { "EVM-2", "ppi.80211n-mac-phy.evm2",
         FT_UINT32, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Error Vector Magnitude (EVM) for chain 2", HFILL } },
    { &hf_80211n_mac_phy_evm3,
       { "EVM-3", "ppi.80211n-mac-phy.evm3",
         FT_UINT32, BASE_DEC, NULL, 0x0, "PPI 802.11n MAC+PHY Error Vector Magnitude (EVM) for chain 3", HFILL } },

    { &hf_ampdu_segment,
        { "A-MPDU", "ppi.80211n-mac.ampdu",
            FT_FRAMENUM, BASE_NONE, NULL, 0x0, "802.11n Aggregated MAC Protocol Data Unit (A-MPDU)", HFILL }},
#if 0
    { &hf_ampdu_segments,
        { "Reassembled A-MPDU", "ppi.80211n-mac.ampdu.reassembled",
            FT_NONE, BASE_NONE, NULL, 0x0, "Reassembled Aggregated MAC Protocol Data Unit (A-MPDU)", HFILL }},
#endif
    { &hf_ampdu_reassembled_in,
        { "Reassembled A-MPDU in frame", "ppi.80211n-mac.ampdu.reassembled_in",
            FT_FRAMENUM, BASE_NONE, NULL, 0x0,
            "The A-MPDU that doesn't end in this segment is reassembled in this frame",
            HFILL }},
    { &hf_ampdu_count,
        { "MPDU count", "ppi.80211n-mac.ampdu.count",
            FT_UINT16, BASE_DEC, NULL, 0x0, "The number of aggregated MAC Protocol Data Units (MPDUs)", HFILL }},

    { &hf_spectrum_map,
       { "Radio spectrum map", "ppi.spectrum-map",
            FT_BYTES, BASE_NONE, NULL, 0x0, "PPI Radio spectrum map", HFILL } },
    { &hf_process_info,
       { "Process information", "ppi.proc-info",
            FT_BYTES, BASE_NONE, NULL, 0x0, "PPI Process information", HFILL } },
    { &hf_capture_info,
       { "Capture information", "ppi.cap-info",
            FT_BYTES, BASE_NONE, NULL, 0x0, "PPI Capture information", HFILL } },

     
    { &hf_aggregation_extension_interface_id,
       { "Interface ID", "ppi.aggregation_extension.interface_id",
            FT_UINT32, BASE_DEC, NULL, 0x0, "Zero-based index of the physical interface the packet was captured from", HFILL } },

     
    { &hf_8023_extension_flags,
       { "Flags", "ppi.8023_extension.flags",
            FT_UINT32, BASE_HEX, NULL, 0x0, "PPI 802.3 Extension Flags", HFILL } },
    { &hf_8023_extension_flags_fcs_present,
       { "FCS Present Flag", "ppi.8023_extension.flags.fcs_present",
            FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x0001, "FCS (4 bytes) is present at the end of the packet", HFILL } },
    { &hf_8023_extension_errors,
       { "Errors", "ppi.8023_extension.errors",
            FT_UINT32, BASE_HEX, NULL, 0x0, "PPI 802.3 Extension Errors", HFILL } },
    { &hf_8023_extension_errors_fcs,
       { "FCS Error", "ppi.8023_extension.errors.fcs",
            FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x0001,
            "PPI 802.3 Extension FCS Error", HFILL } },
    { &hf_8023_extension_errors_sequence,
       { "Sequence Error", "ppi.8023_extension.errors.sequence",
            FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x0002,
            "PPI 802.3 Extension Sequence Error", HFILL } },
    { &hf_8023_extension_errors_symbol,
       { "Symbol Error", "ppi.8023_extension.errors.symbol",
            FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x0004,
            "PPI 802.3 Extension Symbol Error", HFILL } },
    { &hf_8023_extension_errors_data,
       { "Data Error", "ppi.8023_extension.errors.data",
            FT_BOOLEAN, 32, TFS(&tfs_true_false), 0x0008,
            "PPI 802.3 Extension Data Error", HFILL } },

       
      { &hf_ppi_gps, { "GPS", "ppi.gps", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
      { &hf_ppi_vector, { "VECTOR", "ppi.vector", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
      { &hf_ppi_harris, { "HARRIS", "ppi.harris", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
      { &hf_ppi_antenna, { "ANTENNA", "ppi.antenna", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
      { &hf_ppi_fnet, { "FNET", "ppi.fnet", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
      { &hf_ppi_reserved, { "Reserved", "ppi.reserved", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL }},
    };

    static gint *ett[] = {
        &ett_ppi_pph,
        &ett_ppi_flags,
        &ett_dot11_common,
        &ett_dot11_common_flags,
        &ett_dot11_common_channel_flags,
        &ett_dot11n_mac,
        &ett_dot11n_mac_flags,
        &ett_dot11n_mac_phy,
        &ett_dot11n_mac_phy_ext_channel_flags,
        &ett_ampdu_segments,
        &ett_ampdu,
        &ett_ampdu_segment,
        &ett_aggregation_extension,
        &ett_8023_extension,
        &ett_8023_extension_flags,
        &ett_8023_extension_errors
    };

    static ei_register_info ei[] = {
        { &ei_ppi_invalid_length, { "ppi.invalid_length", PI_MALFORMED, PI_ERROR, "Invalid length", EXPFILL }},
    };

    module_t *ppi_module;
    expert_module_t* expert_ppi;

    proto_ppi = proto_register_protocol("PPI Packet Header", "PPI", "ppi");
    proto_register_field_array(proto_ppi, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));
    expert_ppi = expert_register_protocol(proto_ppi);
    expert_register_field_array(expert_ppi, ei, array_length(ei));

    ppi_handle = register_dissector("ppi", dissect_ppi, proto_ppi);

    register_init_routine(ampdu_reassemble_init);
    register_cleanup_routine(ampdu_reassemble_cleanup);

     
    ppi_module = prefs_register_protocol(proto_ppi, NULL);
    prefs_register_bool_preference(ppi_module, "reassemble",
                                   "Reassemble fragmented 802.11 A-MPDUs",
                                   "Whether fragmented 802.11 aggregated MPDUs should be reassembled",
                                   &ppi_ampdu_reassemble);
}
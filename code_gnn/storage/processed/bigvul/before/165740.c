void ParseCommandLineAndFieldTrials(const base::CommandLine& command_line,
                                    bool is_quic_force_disabled,
                                    const std::string& quic_user_agent_id,
                                    net::HttpNetworkSession::Params* params) {
  is_quic_force_disabled |= command_line.HasSwitch(switches::kDisableQuic);
  bool is_quic_force_enabled = command_line.HasSwitch(switches::kEnableQuic);

  std::string quic_trial_group =
      base::FieldTrialList::FindFullName(kQuicFieldTrialName);
  VariationParameters quic_trial_params;
  if (!variations::GetVariationParams(kQuicFieldTrialName, &quic_trial_params))
    quic_trial_params.clear();
  ConfigureQuicParams(quic_trial_group, quic_trial_params,
                      is_quic_force_disabled, is_quic_force_enabled,
                      quic_user_agent_id, params);

  std::string http2_trial_group =
      base::FieldTrialList::FindFullName(kHttp2FieldTrialName);
  VariationParameters http2_trial_params;
  if (!variations::GetVariationParams(kHttp2FieldTrialName,
                                      &http2_trial_params))
    http2_trial_params.clear();
  ConfigureHttp2Params(command_line, http2_trial_group, http2_trial_params,
                       params);

  if (command_line.HasSwitch(switches::kDisableHttp2))
    params->enable_http2 = false;

  if (params->enable_quic) {
    if (command_line.HasSwitch(switches::kQuicConnectionOptions)) {
      params->quic_connection_options = net::ParseQuicConnectionOptions(
          command_line.GetSwitchValueASCII(switches::kQuicConnectionOptions));
    }

    if (command_line.HasSwitch(switches::kQuicMaxPacketLength)) {
      unsigned value;
      if (base::StringToUint(
              command_line.GetSwitchValueASCII(switches::kQuicMaxPacketLength),
              &value)) {
        params->quic_max_packet_length = value;
      }
    }

    if (command_line.HasSwitch(switches::kQuicVersion)) {
      quic::QuicTransportVersionVector supported_versions =
          network_session_configurator::ParseQuicVersions(
              command_line.GetSwitchValueASCII(switches::kQuicVersion));
      if (!supported_versions.empty())
        params->quic_supported_versions = supported_versions;
    }

    if (command_line.HasSwitch(switches::kOriginToForceQuicOn)) {
      std::string origins =
          command_line.GetSwitchValueASCII(switches::kOriginToForceQuicOn);
      for (const std::string& host_port : base::SplitString(
               origins, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL)) {
        if (host_port == "*")
          params->origins_to_force_quic_on.insert(net::HostPortPair());
        net::HostPortPair quic_origin =
            net::HostPortPair::FromString(host_port);
        if (!quic_origin.IsEmpty())
          params->origins_to_force_quic_on.insert(quic_origin);
      }
    }
  }

  if (command_line.HasSwitch(switches::kEnableUserAlternateProtocolPorts)) {
    params->enable_user_alternate_protocol_ports = true;
  }
  if (command_line.HasSwitch(switches::kIgnoreCertificateErrors)) {
    params->ignore_certificate_errors = true;
  }
  UMA_HISTOGRAM_BOOLEAN(
      "Net.Certificate.IgnoreErrors",
      command_line.HasSwitch(switches::kIgnoreCertificateErrors));
  if (command_line.HasSwitch(switches::kTestingFixedHttpPort)) {
    params->testing_fixed_http_port =
        GetSwitchValueAsInt(command_line, switches::kTestingFixedHttpPort);
  }
  if (command_line.HasSwitch(switches::kTestingFixedHttpsPort)) {
    params->testing_fixed_https_port =
        GetSwitchValueAsInt(command_line, switches::kTestingFixedHttpsPort);
  }

  if (command_line.HasSwitch(switches::kHostRules)) {
    params->host_mapping_rules.SetRulesFromString(
        command_line.GetSwitchValueASCII(switches::kHostRules));
  }
}

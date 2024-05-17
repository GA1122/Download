void IOThread::ConfigureQuic(const CommandLine& command_line) {
  std::string quic_trial_group =
      base::FieldTrialList::FindFullName(kQuicFieldTrialName);

  bool enable_quic = ShouldEnableQuic(command_line, quic_trial_group);
  globals_->enable_quic.set(enable_quic);
  if (enable_quic) {
    globals_->enable_quic_https.set(
        ShouldEnableQuicHttps(command_line, quic_trial_group));
  }

  size_t max_packet_length = GetQuicMaxPacketLength(command_line,
                                                    quic_trial_group);
  if (max_packet_length != 0) {
    globals_->quic_max_packet_length.set(max_packet_length);
  }

  if (command_line.HasSwitch(switches::kOriginToForceQuicOn)) {
    net::HostPortPair quic_origin =
        net::HostPortPair::FromString(
            command_line.GetSwitchValueASCII(switches::kOriginToForceQuicOn));
    if (!quic_origin.IsEmpty()) {
      globals_->origin_to_force_quic_on.set(quic_origin);
    }
  }
}

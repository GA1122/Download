size_t IOThread::GetQuicMaxPacketLength(const CommandLine& command_line,
                                        base::StringPiece quic_trial_group) {
  if (command_line.HasSwitch(switches::kQuicMaxPacketLength)) {
    unsigned value;
    if (!base::StringToUint(
            command_line.GetSwitchValueASCII(switches::kQuicMaxPacketLength),
            &value)) {
      return 0;
    }
    return value;
  }

  base::StringPiece length_str(quic_trial_group);
  if (length_str.starts_with(kQuicFieldTrialEnabledGroupName)) {
    length_str.remove_prefix(strlen(kQuicFieldTrialEnabledGroupName));
  } else if (length_str.starts_with(kQuicFieldTrialHttpsEnabledGroupName)) {
    length_str.remove_prefix(strlen(kQuicFieldTrialHttpsEnabledGroupName));
  } else {
    return 0;
  }
  if (!length_str.ends_with(kQuicFieldTrialPacketLengthSuffix)) {
    return 0;
  }
  length_str.remove_suffix(strlen(kQuicFieldTrialPacketLengthSuffix));
  unsigned value;
  if (!base::StringToUint(length_str, &value)) {
    return 0;
  }
  return value;
}

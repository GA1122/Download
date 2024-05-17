bool IOThread::ShouldEnableQuic(const CommandLine& command_line,
                                base::StringPiece quic_trial_group) {
  if (command_line.HasSwitch(switches::kDisableQuic))
    return false;

  if (command_line.HasSwitch(switches::kEnableQuic))
    return true;

  return quic_trial_group.starts_with(kQuicFieldTrialEnabledGroupName) ||
      quic_trial_group.starts_with(kQuicFieldTrialHttpsEnabledGroupName);
}

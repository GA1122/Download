bool IOThread::ShouldEnableQuicHttps(const CommandLine& command_line,
                                     base::StringPiece quic_trial_group) {
  if (command_line.HasSwitch(switches::kDisableQuicHttps))
    return false;

  if (command_line.HasSwitch(switches::kEnableQuicHttps))
    return true;

  return quic_trial_group.starts_with(kQuicFieldTrialHttpsEnabledGroupName);
}

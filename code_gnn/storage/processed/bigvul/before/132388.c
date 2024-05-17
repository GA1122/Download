void ConvertFlagsToSwitches(FlagsStorage* flags_storage,
                            base::CommandLine* command_line,
                            SentinelsMode sentinels) {
  FlagsState::GetInstance()->ConvertFlagsToSwitches(flags_storage,
                                                    command_line,
                                                    sentinels);
}

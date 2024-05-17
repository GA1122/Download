void ConvertFlagsToSwitches(PrefService* prefs, CommandLine* command_line) {
  FlagsState::GetInstance()->ConvertFlagsToSwitches(prefs, command_line);
}

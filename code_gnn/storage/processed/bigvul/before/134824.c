CommandLine::StringType GetSwitchString(const std::string& flag) {
  CommandLine cmd_line(CommandLine::NO_PROGRAM);
  cmd_line.AppendSwitch(flag);
  DCHECK(cmd_line.argv().size() == 2);
  return cmd_line.argv()[1];
}

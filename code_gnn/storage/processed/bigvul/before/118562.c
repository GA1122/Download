CommandLine::StringType GetSwitchString(const std::string& flag) {
  CommandLine cmd_line(CommandLine::NO_PROGRAM);
  cmd_line.AppendSwitch(flag);
  DCHECK_EQ(2U, cmd_line.argv().size());
  return cmd_line.argv()[1];
}

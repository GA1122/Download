base::CommandLine::StringType GetSwitchString(const std::string& flag) {
  base::CommandLine cmd_line(base::CommandLine::NO_PROGRAM);
  cmd_line.AppendSwitch(flag);
  DCHECK_EQ(2U, cmd_line.argv().size());
  return cmd_line.argv()[1];
}

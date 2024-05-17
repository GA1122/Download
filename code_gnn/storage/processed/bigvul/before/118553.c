bool AreSwitchesIdenticalToCurrentCommandLine(
    const CommandLine& new_cmdline, const CommandLine& active_cmdline) {
  std::set<CommandLine::StringType> new_flags =
      ExtractFlagsFromCommandLine(new_cmdline);
  std::set<CommandLine::StringType> active_flags =
      ExtractFlagsFromCommandLine(active_cmdline);

  if (new_flags.size() != active_flags.size())
    return false;

  return std::equal(new_flags.begin(), new_flags.end(), active_flags.begin());
}

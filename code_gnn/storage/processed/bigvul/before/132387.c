bool AreSwitchesIdenticalToCurrentCommandLine(
    const base::CommandLine& new_cmdline,
    const base::CommandLine& active_cmdline,
    std::set<base::CommandLine::StringType>* out_difference) {
  std::set<base::CommandLine::StringType> new_flags =
      ExtractFlagsFromCommandLine(new_cmdline);
  std::set<base::CommandLine::StringType> active_flags =
      ExtractFlagsFromCommandLine(active_cmdline);

  bool result = false;
  if (new_flags.size() == active_flags.size()) {
    result =
        std::equal(new_flags.begin(), new_flags.end(), active_flags.begin());
  }

  if (out_difference && !result) {
    std::set_symmetric_difference(
        new_flags.begin(),
        new_flags.end(),
        active_flags.begin(),
        active_flags.end(),
        std::inserter(*out_difference, out_difference->begin()));
  }

  return result;
}

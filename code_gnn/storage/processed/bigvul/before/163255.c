CreateWinMemoryPressureMonitor(const base::CommandLine& parsed_command_line) {
  std::vector<std::string> thresholds =
      base::SplitString(parsed_command_line.GetSwitchValueASCII(
                            switches::kMemoryPressureThresholdsMb),
                        ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);

  int moderate_threshold_mb = 0;
  int critical_threshold_mb = 0;
  if (thresholds.size() == 2 &&
      base::StringToInt(thresholds[0], &moderate_threshold_mb) &&
      base::StringToInt(thresholds[1], &critical_threshold_mb) &&
      moderate_threshold_mb >= critical_threshold_mb &&
      critical_threshold_mb >= 0) {
    return std::make_unique<base::win::MemoryPressureMonitor>(
        moderate_threshold_mb, critical_threshold_mb);
  }

  return std::make_unique<base::win::MemoryPressureMonitor>();
}

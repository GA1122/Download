void ReportCustomFlags(const std::string& uma_histogram_hame,
                       const std::set<std::string>& command_line_difference) {
  for (const std::string& flag : command_line_difference) {
    int uma_id = about_flags::testing::kBadSwitchFormatHistogramId;
    if (StartsWithASCII(flag, "--", true  )) {
      std::string switch_name(flag.substr(2));

      const size_t value_pos = switch_name.find('=');
      if (value_pos != std::string::npos)
        switch_name.resize(value_pos);

      uma_id = GetSwitchUMAId(switch_name);
    } else {
      NOTREACHED() << "ReportCustomFlags(): flag '" << flag
                   << "' has incorrect format.";
    }
    DVLOG(1) << "ReportCustomFlags(): histogram='" << uma_histogram_hame
             << "' '" << flag << "', uma_id=" << uma_id;

    UMA_HISTOGRAM_SPARSE_SLOWLY(uma_histogram_hame, uma_id);
  }
}

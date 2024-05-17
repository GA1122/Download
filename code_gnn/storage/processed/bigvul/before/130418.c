void ThreadWatcherList::ParseCommandLineCrashOnHangThreads(
    const std::string& crash_on_hang_thread_names,
    uint32 default_live_threads_threshold,
    uint32 default_crash_seconds,
    CrashOnHangThreadMap* crash_on_hang_threads) {
  base::StringTokenizer tokens(crash_on_hang_thread_names, ",");
  while (tokens.GetNext()) {
    std::vector<base::StringPiece> values = base::SplitStringPiece(
        tokens.token_piece(), ":", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    std::string thread_name = values[0].as_string();

    uint32 live_threads_threshold = default_live_threads_threshold;
    uint32 crash_seconds = default_crash_seconds;
    if (values.size() >= 2 &&
        (!base::StringToUint(values[1], &live_threads_threshold))) {
      continue;
    }
    if (values.size() >= 3 &&
        (!base::StringToUint(values[2], &crash_seconds))) {
      continue;
    }
    uint32 unresponsive_threshold = static_cast<uint32>(
        ceil(static_cast<float>(crash_seconds) / kUnresponsiveSeconds));

    CrashDataThresholds crash_data(live_threads_threshold,
                                   unresponsive_threshold);
    (*crash_on_hang_threads)[thread_name] = crash_data;
  }
}

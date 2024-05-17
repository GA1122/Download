void BrowserMainLoop::InitStartupTracingForDuration(
    const base::CommandLine& command_line) {
  DCHECK(is_tracing_startup_for_duration_);

  startup_trace_file_ = GetStartupTraceFileName(parsed_command_line_);

  int delay_secs = 5;
  if (command_line.HasSwitch(switches::kTraceStartup)) {
    std::string delay_str = command_line.GetSwitchValueASCII(
        switches::kTraceStartupDuration);
    if (!delay_str.empty() && !base::StringToInt(delay_str, &delay_secs)) {
      DLOG(WARNING) << "Could not parse --" << switches::kTraceStartupDuration
          << "=" << delay_str << " defaulting to 5 (secs)";
      delay_secs = 5;
    }
  } else {
    delay_secs = tracing::TraceConfigFile::GetInstance()->GetStartupDuration();
  }

  startup_trace_timer_.Start(FROM_HERE,
                             base::TimeDelta::FromSeconds(delay_secs),
                             this,
                             &BrowserMainLoop::EndStartupTracing);
}

base::FilePath BrowserMainLoop::GetStartupTraceFileName(
    const base::CommandLine& command_line) const {
  base::FilePath trace_file;
  if (command_line.HasSwitch(switches::kTraceStartup)) {
    trace_file = command_line.GetSwitchValuePath(
        switches::kTraceStartupFile);
    if (trace_file == base::FilePath().AppendASCII("none"))
      return trace_file;

    if (trace_file.empty()) {
#if defined(OS_ANDROID)
      TracingControllerAndroid::GenerateTracingFilePath(&trace_file);
#else
      trace_file = base::FilePath().AppendASCII("chrometrace.log");
#endif
    }
  } else {
#if defined(OS_ANDROID)
    TracingControllerAndroid::GenerateTracingFilePath(&trace_file);
#else
    trace_file = tracing::TraceConfigFile::GetInstance()->GetResultFile();
#endif
  }

  return trace_file;
}

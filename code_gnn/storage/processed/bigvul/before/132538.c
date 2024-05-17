void ShellContentBrowserClient::AppendExtraCommandLineSwitches(
    base::CommandLine* command_line,
    int child_process_id) {
#if defined(OS_POSIX) && !defined(OS_MACOSX)
#if defined(V8_USE_EXTERNAL_STARTUP_DATA)
  std::string process_type =
      command_line->GetSwitchValueASCII(switches::kProcessType);
  if (process_type != switches::kZygoteProcess) {
    command_line->AppendSwitch(::switches::kV8NativesPassedByFD);
    command_line->AppendSwitch(::switches::kV8SnapshotPassedByFD);
  }
#endif   
#endif   
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kRunLayoutTest))
    command_line->AppendSwitch(switches::kRunLayoutTest);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFontAntialiasing))
    command_line->AppendSwitch(switches::kEnableFontAntialiasing);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kExposeInternalsForTesting))
    command_line->AppendSwitch(switches::kExposeInternalsForTesting);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kExposeIpcEcho))
    command_line->AppendSwitch(switches::kExposeIpcEcho);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kStableReleaseMode))
    command_line->AppendSwitch(switches::kStableReleaseMode);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableCrashReporter)) {
    command_line->AppendSwitch(switches::kEnableCrashReporter);
  }
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kCrashDumpsDir)) {
    command_line->AppendSwitchPath(
        switches::kCrashDumpsDir,
        base::CommandLine::ForCurrentProcess()->GetSwitchValuePath(
            switches::kCrashDumpsDir));
  }
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableLeakDetection)) {
    command_line->AppendSwitchASCII(
        switches::kEnableLeakDetection,
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            switches::kEnableLeakDetection));
  }
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kRegisterFontFiles)) {
    command_line->AppendSwitchASCII(
        switches::kRegisterFontFiles,
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            switches::kRegisterFontFiles));
  }
}

void ShellMainDelegate::PreSandboxStartup() {
#if defined(ARCH_CPU_ARM_FAMILY) && (defined(OS_ANDROID) || defined(OS_LINUX))
  base::CPU cpu_info;
#endif

#if !defined(OS_FUCHSIA)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableCrashReporter)) {
    std::string process_type =
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            switches::kProcessType);
    crash_reporter::SetCrashReporterClient(g_shell_crash_client.Pointer());
#if defined(OS_MACOSX) || defined(OS_WIN)
    crash_reporter::InitializeCrashpad(process_type.empty(), process_type);
#elif defined(OS_LINUX)
    if (process_type != service_manager::switches::kZygoteProcess)
      breakpad::InitCrashReporter(process_type);
#elif defined(OS_ANDROID)
    if (process_type.empty())
      breakpad::InitCrashReporter(process_type);
    else
      breakpad::InitNonBrowserCrashReporterForAndroid(process_type);
#endif   
  }
#endif   

  crash_reporter::InitializeCrashKeys();

  InitializeResourceBundle();
}

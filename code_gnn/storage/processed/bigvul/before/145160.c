  explicit GpuSandboxedProcessLauncherDelegate(
      const base::CommandLine& cmd_line)
#if defined(OS_WIN)
      : cmd_line_(cmd_line),
        enable_appcontainer_(true)
#endif
  {
  }

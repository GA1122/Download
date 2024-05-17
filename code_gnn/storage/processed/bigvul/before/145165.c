bool GpuProcessHost::LaunchGpuProcess() {
  const base::CommandLine& browser_command_line =
      *base::CommandLine::ForCurrentProcess();

  base::CommandLine::StringType gpu_launcher =
      browser_command_line.GetSwitchValueNative(switches::kGpuLauncher);

#if defined(OS_ANDROID)
  std::unique_ptr<base::CommandLine> cmd_line =
      std::make_unique<base::CommandLine>(base::CommandLine::NO_PROGRAM);
#else
#if defined(OS_LINUX)
  int child_flags = gpu_launcher.empty() ? ChildProcessHost::CHILD_ALLOW_SELF :
                                           ChildProcessHost::CHILD_NORMAL;
#else
  int child_flags = ChildProcessHost::CHILD_NORMAL;
#endif

  base::FilePath exe_path = ChildProcessHost::GetChildPath(child_flags);
  if (exe_path.empty())
    return false;

  std::unique_ptr<base::CommandLine> cmd_line =
      std::make_unique<base::CommandLine>(exe_path);
#endif

  cmd_line->AppendSwitchASCII(switches::kProcessType, switches::kGpuProcess);

  BrowserChildProcessHostImpl::CopyFeatureAndFieldTrialFlags(cmd_line.get());
  BrowserChildProcessHostImpl::CopyTraceStartupFlags(cmd_line.get());

#if defined(OS_WIN)
  cmd_line->AppendArg(switches::kPrefetchArgumentGpu);
#endif   

  if (kind_ == GPU_PROCESS_KIND_UNSANDBOXED)
    cmd_line->AppendSwitch(service_manager::switches::kDisableGpuSandbox);

  cmd_line->CopySwitchesFrom(browser_command_line, kSwitchNames,
                             arraysize(kSwitchNames));
  cmd_line->CopySwitchesFrom(
      browser_command_line, switches::kGLSwitchesCopiedFromGpuProcessHost,
      switches::kGLSwitchesCopiedFromGpuProcessHostNumSwitches);

  if (browser_command_line.HasSwitch(switches::kDisableFrameRateLimit))
    cmd_line->AppendSwitch(switches::kDisableGpuVsync);

  std::vector<const char*> gpu_workarounds;
  gpu::GpuDriverBugList::AppendAllWorkarounds(&gpu_workarounds);
  cmd_line->CopySwitchesFrom(browser_command_line, gpu_workarounds.data(),
                             gpu_workarounds.size());

  GetContentClient()->browser()->AppendExtraCommandLineSwitches(
      cmd_line.get(), process_->GetData().id);

  GpuDataManagerImpl::GetInstance()->AppendGpuCommandLine(cmd_line.get());
  bool swiftshader_rendering =
      (cmd_line->GetSwitchValueASCII(switches::kUseGL) ==
       gl::kGLImplementationSwiftShaderForWebGLName);

  UMA_HISTOGRAM_BOOLEAN("GPU.GPUProcessSoftwareRendering",
                        swiftshader_rendering);

  if (!gpu_launcher.empty())
    cmd_line->PrependWrapper(gpu_launcher);

  std::unique_ptr<GpuSandboxedProcessLauncherDelegate> delegate =
      std::make_unique<GpuSandboxedProcessLauncherDelegate>(*cmd_line);
#if defined(OS_WIN)
  if (crashed_before_)
    delegate->DisableAppContainer();
#endif   
  process_->Launch(std::move(delegate), std::move(cmd_line), true);
  process_launched_ = true;

  if (kind_ == GPU_PROCESS_KIND_SANDBOXED) {
    if (mode_ == gpu::GpuMode::HARDWARE_ACCELERATED) {
      UMA_HISTOGRAM_ENUMERATION(kProcessLifetimeEventsHardwareAccelerated,
                                LAUNCHED, GPU_PROCESS_LIFETIME_EVENT_MAX);
    } else if (mode_ == gpu::GpuMode::SWIFTSHADER) {
      UMA_HISTOGRAM_ENUMERATION(kProcessLifetimeEventsSwiftShader, LAUNCHED,
                                GPU_PROCESS_LIFETIME_EVENT_MAX);
    } else if (mode_ == gpu::GpuMode::DISPLAY_COMPOSITOR) {
      UMA_HISTOGRAM_ENUMERATION(kProcessLifetimeEventsDisplayCompositor,
                                LAUNCHED, GPU_PROCESS_LIFETIME_EVENT_MAX);
    }
  }

  return true;
}

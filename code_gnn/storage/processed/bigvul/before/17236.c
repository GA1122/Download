void InitializeCommandLine(const std::string& argv0,
                           const base::FilePath& subprocess_path,
                           ProcessModel process_model,
                           gl::GLImplementation gl_impl,
                           base::Environment* env) {
  const char* argv0_c = nullptr;
  if (!argv0.empty()) {
    argv0_c = argv0.c_str();
  }
  CHECK(base::CommandLine::Init(argv0_c ? 1 : 0,
                                argv0_c ? &argv0_c : nullptr)) <<
      "CommandLine already exists. Did you call BrowserProcessMain::Start "
      "in a child process?";

  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();

  command_line->AppendSwitchASCII(switches::kBrowserSubprocessPath,
                                  subprocess_path.value().c_str());

  command_line->AppendSwitch(switches::kInProcessGPU);

  command_line->AppendSwitch(switches::kDisableGpuShaderDiskCache);

  command_line->AppendSwitch(switches::kDisableGpuEarlyInit);

  command_line->AppendSwitch(switches::kUIPrioritizeInGpuProcess);
  command_line->AppendSwitch(switches::kEnableSmoothScrolling);

  command_line->AppendSwitchASCII(switches::kProfilerTiming,
                                  switches::kProfilerTimingDisabledValue);

  if (gl_impl == gl::kGLImplementationNone ||
      IsEnvironmentOptionEnabled("DISABLE_GPU", env)) {
    command_line->AppendSwitch(switches::kDisableGpu);
  } else {
    command_line->AppendSwitchASCII(switches::kUseGL,
                                    GetGLImplName(gl_impl));
  }

  if (IsEnvironmentOptionEnabled("DISABLE_GPU_COMPOSITING", env)) {
    command_line->AppendSwitch(switches::kDisableGpuCompositing);
  }

  std::string renderer_cmd_prefix =
      GetEnvironmentOption("RENDERER_CMD_PREFIX", env);
  if (!renderer_cmd_prefix.empty()) {
    command_line->AppendSwitchASCII(switches::kRendererCmdPrefix,
                                    renderer_cmd_prefix);
  }
  if (IsEnvironmentOptionEnabled("NO_SANDBOX", env)) {
    command_line->AppendSwitch(switches::kNoSandbox);
  } else {
    command_line->AppendSwitch(switches::kDisableNamespaceSandbox);

    if (IsEnvironmentOptionEnabled("DISABLE_SETUID_SANDBOX", env)) {
      command_line->AppendSwitch(switches::kDisableSetuidSandbox);
    }
    if (IsEnvironmentOptionEnabled("DISABLE_SECCOMP_FILTER_SANDBOX",
                                   env)) {
      command_line->AppendSwitch(switches::kDisableSeccompFilterSandbox);
    }
  }

  if (IsEnvironmentOptionEnabled("IGNORE_GPU_BLACKLIST", env)) {
    command_line->AppendSwitch(switches::kIgnoreGpuBlacklist);
  }
  if (IsEnvironmentOptionEnabled("DISABLE_GPU_DRIVER_BUG_WORKAROUNDS",
                                 env)) {
    command_line->AppendSwitch(switches::kDisableGpuDriverBugWorkarounds);
  }

  if (IsEnvironmentOptionEnabled("ENABLE_GPU_SERVICE_LOGGING", env)) {
    command_line->AppendSwitch(switches::kEnableGPUServiceLogging);
  }
  if (IsEnvironmentOptionEnabled("ENABLE_GPU_DEBUGGING", env)) {
    command_line->AppendSwitch(switches::kEnableGPUDebugging);
  }

  if (process_model == PROCESS_MODEL_SINGLE_PROCESS) {
    command_line->AppendSwitch(switches::kSingleProcess);
  } else if (process_model == PROCESS_MODEL_PROCESS_PER_VIEW) {
    command_line->AppendSwitch(switches::kProcessPerTab);
  } else if (process_model == PROCESS_MODEL_PROCESS_PER_SITE) {
    command_line->AppendSwitch(switches::kProcessPerSite);
  } else if (process_model == PROCESS_MODEL_SITE_PER_PROCESS) {
    command_line->AppendSwitch(switches::kSitePerProcess);
  } else {
    DCHECK(process_model == PROCESS_MODEL_PROCESS_PER_SITE_INSTANCE ||
           process_model == PROCESS_MODEL_MULTI_PROCESS);
  }

  if (IsEnvironmentOptionEnabled("ALLOW_SANDBOX_DEBUGGING", env)) {
    command_line->AppendSwitch(switches::kAllowSandboxDebugging);
  }

  if (IsEnvironmentOptionEnabled("ENABLE_MEDIA_HUB_AUDIO", env)) {
    command_line->AppendSwitch(switches::kEnableMediaHubAudio);
  }
  std::string mediahub_fixed_session_domains =
      GetEnvironmentOption("MEDIA_HUB_FIXED_SESSION_DOMAINS", env);
  if (!mediahub_fixed_session_domains.empty()) {
    command_line->AppendSwitchASCII(switches::kMediaHubFixedSessionDomains,
                                    mediahub_fixed_session_domains);
    if (!IsEnvironmentOptionEnabled("ENABLE_MEDIA_HUB_AUDIO", env)) {
      command_line->AppendSwitch(switches::kEnableMediaHubAudio);
    }
  }

  if (IsEnvironmentOptionEnabled("TESTING_MODE", env)) {
    command_line->AppendSwitch(switches::kUseFakeDeviceForMediaStream);
  }

#if defined(OS_POSIX)
  command_line->AppendSwitchPath(switches::kSharedMemoryOverridePath,
                                 GetSharedMemoryPath(env));
#endif

  const std::string& verbose_log_level =
      GetEnvironmentOption("VERBOSE_LOG_LEVEL", env);
  if (!verbose_log_level.empty()) {
    command_line->AppendSwitch(switches::kEnableLogging);
    command_line->AppendSwitchASCII(switches::kV, verbose_log_level);
  }

  const std::string& extra_cmd_arg_list =
      GetEnvironmentOption("EXTRA_CMD_ARGS", env);
  if (!extra_cmd_arg_list.empty()) {
    std::vector<std::string> args =
        base::SplitString(extra_cmd_arg_list,
                          base::kWhitespaceASCII,
                          base::KEEP_WHITESPACE,
                          base::SPLIT_WANT_NONEMPTY);

    base::CommandLine::StringVector new_args;
    new_args.push_back(command_line->argv()[0]);
    new_args.insert(new_args.end(), args.begin(), args.end());

    base::CommandLine extra_cmd_line(new_args);
    command_line->AppendArguments(extra_cmd_line, false);
  }
}

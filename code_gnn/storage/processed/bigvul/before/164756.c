void ChildProcessLauncherHelper::BeforeLaunchOnClientThread() {
  std::string process_type =
      command_line()->GetSwitchValueASCII(switches::kProcessType);
  CHECK(process_type == switches::kGpuProcess ||
        process_type == switches::kRendererProcess ||
        process_type == switches::kUtilityProcess)
      << "Unsupported process type: " << process_type;

  DCHECK(process_type == switches::kGpuProcess ||
         !command_line()->HasSwitch(service_manager::switches::kNoSandbox));
}

int GetCrashSignalFD(const base::CommandLine& command_line) {
  if (command_line.HasSwitch(extensions::switches::kExtensionProcess)) {
    static breakpad::CrashHandlerHostLinux* crash_handler = NULL;
    if (!crash_handler)
      crash_handler = CreateCrashHandlerHost("extension");
    return crash_handler->GetDeathSignalSocket();
  }

#if defined(OS_CHROMEOS)
  if (command_line.HasSwitch(switches::kMashServiceName)) {
    static base::NoDestructor<
        std::map<std::string, breakpad::CrashHandlerHostLinux*>>
        crash_handlers;
    std::string service_name =
        command_line.GetSwitchValueASCII(switches::kMashServiceName);
    auto it = crash_handlers->find(service_name);
    if (it == crash_handlers->end()) {
      crash_handlers->insert(
          std::make_pair(service_name, CreateCrashHandlerHost(service_name)));
    }
    return crash_handlers->at(service_name)->GetDeathSignalSocket();
  }
#endif   

  std::string process_type =
      command_line.GetSwitchValueASCII(switches::kProcessType);

  if (process_type == switches::kRendererProcess) {
    static breakpad::CrashHandlerHostLinux* crash_handler = NULL;
    if (!crash_handler)
      crash_handler = CreateCrashHandlerHost(process_type);
    return crash_handler->GetDeathSignalSocket();
  }

  if (process_type == switches::kPpapiPluginProcess) {
    static breakpad::CrashHandlerHostLinux* crash_handler = NULL;
    if (!crash_handler)
      crash_handler = CreateCrashHandlerHost(process_type);
    return crash_handler->GetDeathSignalSocket();
  }

  if (process_type == switches::kGpuProcess) {
    static breakpad::CrashHandlerHostLinux* crash_handler = NULL;
    if (!crash_handler)
      crash_handler = CreateCrashHandlerHost(process_type);
    return crash_handler->GetDeathSignalSocket();
  }

  if (process_type == switches::kUtilityProcess) {
    static breakpad::CrashHandlerHostLinux* crash_handler = nullptr;
    if (!crash_handler)
      crash_handler = CreateCrashHandlerHost(process_type);
    return crash_handler->GetDeathSignalSocket();
  }

  return -1;
}

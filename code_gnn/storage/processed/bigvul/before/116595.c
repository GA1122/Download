bool RenderProcessImpl::InProcessPlugins() {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
#if defined(OS_LINUX) || defined(OS_OPENBSD)
  if (command_line.HasSwitch(switches::kInProcessPlugins))
    NOTIMPLEMENTED() << ": in process plugins not supported on Linux";
  return command_line.HasSwitch(switches::kInProcessPlugins);
#else
  return command_line.HasSwitch(switches::kInProcessPlugins) ||
         command_line.HasSwitch(switches::kSingleProcess);
 #endif
 }

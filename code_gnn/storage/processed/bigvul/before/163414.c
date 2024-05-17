void RenderThreadImpl::Shutdown() {
  ChildThreadImpl::Shutdown();
  quota_dispatcher_.reset();
  file_system_dispatcher_.reset();
  WebFileSystemImpl::DeleteThreadSpecificInstance();

  blink::MainThreadIsolate()->DumpAndResetStats();

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDumpBlinkRuntimeCallStats))
    blink::LogRuntimeCallStats();

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSingleProcess))
    base::Process::TerminateCurrentProcessImmediately(0);
}

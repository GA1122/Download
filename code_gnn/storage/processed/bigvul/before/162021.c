void BrowserChildProcessHostImpl::Launch(
    std::unique_ptr<SandboxedProcessLauncherDelegate> delegate,
    std::unique_ptr<base::CommandLine> cmd_line,
    bool terminate_on_shutdown) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  GetContentClient()->browser()->AppendExtraCommandLineSwitches(cmd_line.get(),
                                                                data_.id);

  const base::CommandLine& browser_command_line =
      *base::CommandLine::ForCurrentProcess();
  static const char* const kForwardSwitches[] = {
      service_manager::switches::kDisableInProcessStackTraces,
      switches::kDisableLogging,
      switches::kEnableLogging,
      switches::kIPCConnectionTimeout,
      switches::kLoggingLevel,
      switches::kTraceToConsole,
      switches::kV,
      switches::kVModule,
  };
  cmd_line->CopySwitchesFrom(browser_command_line, kForwardSwitches,
                             arraysize(kForwardSwitches));

  if (child_connection_) {
    cmd_line->AppendSwitchASCII(switches::kServiceRequestChannelToken,
                                child_connection_->service_token());
  }

  DCHECK(broker_client_invitation_);
  notify_child_disconnected_ = true;
  child_process_.reset(new ChildProcessLauncher(
      std::move(delegate), std::move(cmd_line), data_.id, this,
      std::move(broker_client_invitation_),
      base::Bind(&BrowserChildProcessHostImpl::OnMojoError,
                 weak_factory_.GetWeakPtr(),
                 base::ThreadTaskRunnerHandle::Get()),
      terminate_on_shutdown));
}

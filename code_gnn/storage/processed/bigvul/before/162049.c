void RenderProcessHostImpl::AppendRendererCommandLine(
    base::CommandLine* command_line) {
  command_line->AppendSwitchASCII(switches::kProcessType,
                                  switches::kRendererProcess);

#if defined(OS_WIN)
  command_line->AppendArg(switches::kPrefetchArgumentRenderer);
#endif   

  const base::CommandLine& browser_command_line =
      *base::CommandLine::ForCurrentProcess();
  PropagateBrowserCommandLineToRenderer(browser_command_line, command_line);

  const std::string locale =
      GetContentClient()->browser()->GetApplicationLocale();
  command_line->AppendSwitchASCII(switches::kLang, locale);

  if (!base::CommandLine::ForCurrentProcess()
           ->GetSwitchValueNative(switches::kRendererCmdPrefix)
           .empty()) {
    command_line->AppendSwitch(switches::kNoZygote);
  }

  GetContentClient()->browser()->AppendExtraCommandLineSwitches(command_line,
                                                                GetID());

#if defined(OS_WIN)
  command_line->AppendSwitchASCII(
      switches::kDeviceScaleFactor,
      base::NumberToString(display::win::GetDPIScale()));
#endif

  AppendCompositorCommandLineFlags(command_line);

  command_line->AppendSwitchASCII(switches::kServiceRequestChannelToken,
                                  child_connection_->service_token());
  command_line->AppendSwitchASCII(switches::kRendererClientId,
                                  std::to_string(GetID()));
}

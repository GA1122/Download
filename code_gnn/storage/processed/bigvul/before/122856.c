void RenderProcessHostImpl::AppendRendererCommandLine(
    CommandLine* command_line) const {
  command_line->AppendSwitchASCII(switches::kProcessType,
                                  switches::kRendererProcess);
  if (is_guest_)
    command_line->AppendSwitch(switches::kGuestRenderer);

  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  PropagateBrowserCommandLineToRenderer(browser_command_line, command_line);

  const std::string locale =
      GetContentClient()->browser()->GetApplicationLocale();
  command_line->AppendSwitchASCII(switches::kLang, locale);

  std::string field_trial_states;
  base::FieldTrialList::StatesToString(&field_trial_states);
  if (!field_trial_states.empty()) {
    command_line->AppendSwitchASCII(switches::kForceFieldTrials,
                                    field_trial_states);
  }

  GetContentClient()->browser()->AppendExtraCommandLineSwitches(
      command_line, GetID());

  GpuDataManagerImpl* gpu_data_manager = GpuDataManagerImpl::GetInstance();
  DCHECK(gpu_data_manager);
  gpu_data_manager->AppendRendererCommandLine(command_line);
}

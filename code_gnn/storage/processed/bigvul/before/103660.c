void BrowserRenderProcessHost::AppendRendererCommandLine(
    CommandLine* command_line) const {
  command_line->AppendSwitchASCII(switches::kProcessType,
      is_extension_process_ ? switches::kExtensionProcess :
                              switches::kRendererProcess);

  if (accessibility_enabled_)
    command_line->AppendSwitch(switches::kEnableAccessibility);

  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  PropagateBrowserCommandLineToRenderer(browser_command_line, command_line);

  const std::string locale =
      content::GetContentClient()->browser()->GetApplicationLocale();
  command_line->AppendSwitchASCII(switches::kLang, locale);

  std::string field_trial_states;
  base::FieldTrialList::StatesToString(&field_trial_states);
  if (!field_trial_states.empty()) {
    command_line->AppendSwitchASCII(switches::kForceFieldTestNameAndValue,
                                    field_trial_states);
  }

  content::GetContentClient()->browser()->AppendExtraCommandLineSwitches(
      command_line, id());

  GpuDataManager* gpu_data_manager = GpuDataManager::GetInstance();
  DCHECK(gpu_data_manager);
  gpu_data_manager->AppendRendererCommandLine(command_line);
}

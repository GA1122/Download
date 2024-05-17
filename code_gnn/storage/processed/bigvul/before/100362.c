void BrowserRenderProcessHost::AppendRendererCommandLine(
    CommandLine* command_line,
    bool* has_cmd_prefix) const {
  if (logging::DialogsAreSuppressed())
    command_line->AppendSwitch(switches::kNoErrorDialogs);

  command_line->AppendSwitchWithValue(switches::kProcessType,
                                      switches::kRendererProcess);

  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  PropogateBrowserCommandLineToRenderer(browser_command_line, command_line);

  const std::string locale = g_browser_process->GetApplicationLocale();
  command_line->AppendSwitchWithValue(switches::kLang, ASCIIToWide(locale));

  std::string field_trial_states;
  FieldTrialList::StatesToString(&field_trial_states);
  if (!field_trial_states.empty()) {
    command_line->AppendSwitchWithValue(switches::kForceFieldTestNameAndValue,
        ASCIIToWide(field_trial_states));
  }

#if defined(OS_POSIX)
  *has_cmd_prefix =
      browser_command_line.HasSwitch(switches::kRendererCmdPrefix);
  if (*has_cmd_prefix) {
    const std::wstring prefix =
        browser_command_line.GetSwitchValue(switches::kRendererCmdPrefix);
    command_line->PrependWrapper(prefix);
  }
#else
  *has_cmd_prefix = false;
#endif   

  ChildProcessHost::SetCrashReporterCommandLine(command_line);

  const std::wstring& profile_path =
      browser_command_line.GetSwitchValue(switches::kUserDataDir);
  if (!profile_path.empty())
    command_line->AppendSwitchWithValue(switches::kUserDataDir, profile_path);
}

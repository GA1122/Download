void ChromeBrowserMainParts::PreBrowserStart() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PreBrowserStart");
  for (size_t i = 0; i < chrome_extra_parts_.size(); ++i)
    chrome_extra_parts_[i]->PreBrowserStart();

  three_d_observer_.reset(new ThreeDAPIObserver());

#if defined(OS_CHROMEOS)
  g_browser_process->GetOomPriorityManager()->Start();
#elif defined(OS_WIN) || defined(OS_MACOSX)
  const std::string group_name =
      base::FieldTrialList::FindFullName("AutomaticTabDiscarding");
  if (parsed_command_line().HasSwitch(switches::kEnableTabDiscarding) ||
      base::StartsWith(group_name, "Enabled", base::CompareCase::SENSITIVE)) {
    g_browser_process->GetOomPriorityManager()->Start();
  }
#endif
}

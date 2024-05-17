bool StartupBrowserCreator::Start(const base::CommandLine& cmd_line,
                                  const base::FilePath& cur_dir,
                                  Profile* last_used_profile,
                                  const Profiles& last_opened_profiles) {
  TRACE_EVENT0("startup", "StartupBrowserCreator::Start");
  SCOPED_UMA_HISTOGRAM_TIMER("Startup.StartupBrowserCreator_Start");
  return ProcessCmdLineImpl(cmd_line, cur_dir, true, last_used_profile,
                            last_opened_profiles);
}

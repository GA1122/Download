void InProcessBrowserTest::PrepareTestCommandLine(CommandLine* command_line) {
  test_launcher_utils::PrepareBrowserCommandLineForTests(command_line);

  if (dom_automation_enabled_)
    command_line->AppendSwitch(switches::kDomAutomationController);

  command_line->AppendSwitchASCII(switches::kTestType, kBrowserTestType);

#if defined(OS_WIN)
  command_line->AppendSwitchPath(switches::kBrowserSubprocessPath,
                                 command_line->GetProgram());
#else
  FilePath subprocess_path;
  PathService::Get(base::FILE_EXE, &subprocess_path);
#if defined(OS_MACOSX)
  subprocess_path = subprocess_path.DirName().DirName();
  DCHECK_EQ(subprocess_path.BaseName().value(), "Contents");
  subprocess_path =
      subprocess_path.Append("Versions").Append(chrome::kChromeVersion);
  subprocess_path =
      subprocess_path.Append(chrome::kHelperProcessExecutablePath);
#endif
  command_line->AppendSwitchPath(switches::kBrowserSubprocessPath,
                                 subprocess_path);
#endif

  if (!tab_closeable_state_watcher_enabled_)
    command_line->AppendSwitch(switches::kDisableTabCloseableStateWatcher);
}

int LayoutTestBrowserMain(
    const content::MainFunctionParams& parameters,
    const scoped_ptr<content::BrowserMainRunner>& main_runner) {
  base::ScopedTempDir browser_context_path_for_layout_tests;

  CHECK(browser_context_path_for_layout_tests.CreateUniqueTempDir());
  CHECK(!browser_context_path_for_layout_tests.path().MaybeAsASCII().empty());
  base::CommandLine::ForCurrentProcess()->AppendSwitchASCII(
      switches::kContentShellDataPath,
      browser_context_path_for_layout_tests.path().MaybeAsASCII());

#if defined(OS_ANDROID)
  content::EnsureInitializeForAndroidLayoutTests();
#endif

  int exit_code = main_runner->Initialize(parameters);
  DCHECK_LT(exit_code, 0)
      << "BrowserMainRunner::Initialize failed in LayoutTestBrowserMain";

  if (exit_code >= 0)
    return exit_code;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kCheckLayoutTestSysDeps)) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    main_runner->Run();
    content::Shell::CloseAllWindows();
    main_runner->Shutdown();
    return 0;
  }

  exit_code = RunTests(main_runner);

#if !defined(OS_ANDROID)
  main_runner->Shutdown();
#endif

  return exit_code;
}

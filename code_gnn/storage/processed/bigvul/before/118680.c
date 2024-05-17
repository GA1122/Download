void InProcessBrowserTest::SetUp() {
  DCHECK(!g_browser_process);

  CommandLine* command_line = CommandLine::ForCurrentProcess();

  command_line->AppendSwitch(switches::kDisableOfflineAutoReload);

  SetUpCommandLine(command_line);
  PrepareTestCommandLine(command_line);

  ASSERT_TRUE(CreateUserDataDirectory())
      << "Could not create user data directory.";

  ASSERT_TRUE(SetUpUserDataDirectory())
      << "Could not set up user data directory.";

#if defined(OS_CHROMEOS)
  base::FilePath log_dir = logging::GetSessionLogFile(*command_line).DirName();
  base::CreateDirectory(log_dir);
#endif   

#if defined(OS_MACOSX)
  OSCrypt::UseMockKeychain(true);
#endif

#if defined(ENABLE_CAPTIVE_PORTAL_DETECTION)
  CaptivePortalService::set_state_for_testing(
      CaptivePortalService::DISABLED_FOR_TESTING);
#endif

  chrome_browser_net::NetErrorTabHelper::set_state_for_testing(
      chrome_browser_net::NetErrorTabHelper::TESTING_FORCE_DISABLED);

  google_util::SetMockLinkDoctorBaseURLForTesting();

#if defined(OS_WIN)
  base::win::Version version = base::win::GetVersion();
  if (version >= base::win::VERSION_VISTA &&
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kAshBrowserTests)) {
    com_initializer_.reset(new base::win::ScopedCOMInitializer());
    ui::win::CreateATLModuleIfNeeded();
    if (version >= base::win::VERSION_WIN8)
      ASSERT_TRUE(win8::MakeTestDefaultBrowserSynchronously());
  }
#endif

  BrowserTestBase::SetUp();
}

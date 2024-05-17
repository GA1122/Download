void LiveSyncTest::SetUp() {
  CommandLine* cl = CommandLine::ForCurrentProcess();
  if (cl->HasSwitch(switches::kPasswordFileForTest)) {
    ReadPasswordFile();
  } else if (cl->HasSwitch(switches::kSyncUserForTest) &&
             cl->HasSwitch(switches::kSyncPasswordForTest)) {
    username_ = cl->GetSwitchValueASCII(switches::kSyncUserForTest);
    password_ = cl->GetSwitchValueASCII(switches::kSyncPasswordForTest);
  } else {
    SetupMockGaiaResponses();
  }

   if (!cl->HasSwitch(switches::kSyncServiceURL) &&
       !cl->HasSwitch(switches::kSyncServerCommandLine)) {
    server_type_ = LOCAL_PYTHON_SERVER;
   } else if (cl->HasSwitch(switches::kSyncServiceURL) &&
              cl->HasSwitch(switches::kSyncServerCommandLine)) {
    server_type_ = LOCAL_LIVE_SERVER;
  } else if (cl->HasSwitch(switches::kSyncServiceURL) &&
             !cl->HasSwitch(switches::kSyncServerCommandLine)) {
    server_type_ = EXTERNAL_LIVE_SERVER;
  } else {
    LOG(FATAL) << "Can't figure out how to run a server.";
  }

  if (username_.empty() || password_.empty())
    LOG(FATAL) << "Cannot run sync tests without GAIA credentials.";

#if defined(OS_MACOSX)
  Encryptor::UseMockKeychain(true);
#endif

  InProcessBrowserTest::SetUp();
}

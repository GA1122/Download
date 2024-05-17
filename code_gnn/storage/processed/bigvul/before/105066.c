bool LiveSyncTest::SetUpLocalTestServer() {
  CommandLine* cl = CommandLine::ForCurrentProcess();
  CommandLine::StringType server_cmdline_string = cl->GetSwitchValueNative(
      switches::kSyncServerCommandLine);
  CommandLine::StringVector server_cmdline_vector;
  CommandLine::StringType delimiters(FILE_PATH_LITERAL(" "));
  Tokenize(server_cmdline_string, delimiters, &server_cmdline_vector);
  CommandLine server_cmdline(server_cmdline_vector);
  if (!base::LaunchApp(server_cmdline, false, true, &test_server_handle_))
    LOG(ERROR) << "Could not launch local test server.";

  const int kMaxWaitTime = TestTimeouts::live_operation_timeout_ms();
  const int kNumIntervals = 15;
  if (WaitForTestServerToStart(kMaxWaitTime, kNumIntervals)) {
    VLOG(1) << "Started local test server at "
            << cl->GetSwitchValueASCII(switches::kSyncServiceURL) << ".";
    return true;
  } else {
    LOG(ERROR) << "Could not start local test server at "
               << cl->GetSwitchValueASCII(switches::kSyncServiceURL) << ".";
    return false;
  }
}

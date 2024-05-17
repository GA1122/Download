void ComponentUpdaterPolicyTest::SetUpCommandLine(
    base::CommandLine* command_line) {
  ASSERT_TRUE(https_server_.InitializeAndListen());
  const std::string val = base::StringPrintf(
      "url-source=%s", https_server_.GetURL("/service/update2").spec().c_str());
  command_line->AppendSwitchASCII(switches::kComponentUpdater, val.c_str());
  PolicyTest::SetUpCommandLine(command_line);
}

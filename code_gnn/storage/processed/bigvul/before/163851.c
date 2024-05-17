void ExtensionBrowserTest::SetUpCommandLine(base::CommandLine* command_line) {
  PathService::Get(chrome::DIR_TEST_DATA, &test_data_dir_);
  test_data_dir_ = test_data_dir_.AppendASCII("extensions");

  ExtensionMessageBubbleFactory::set_override_for_tests(
      ExtensionMessageBubbleFactory::OVERRIDE_DISABLED);

#if defined(OS_CHROMEOS)
  if (set_chromeos_user_) {
    command_line->AppendSwitchASCII(chromeos::switches::kLoginUser,
                                    "testuser@gmail.com");
    command_line->AppendSwitchASCII(chromeos::switches::kLoginProfile, "user");
  }
#endif
}

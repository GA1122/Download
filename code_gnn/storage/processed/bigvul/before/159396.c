void ExtensionApiTest::SetUpCommandLine(base::CommandLine* command_line) {
  ExtensionBrowserTest::SetUpCommandLine(command_line);

  test_data_dir_ = test_data_dir_.AppendASCII("api_test");

  extensions::RegisterPathProvider();
  PathService::Get(extensions::DIR_TEST_DATA, &shared_test_data_dir_);
  shared_test_data_dir_ = shared_test_data_dir_.AppendASCII("api_test");

  command_line->AppendSwitch(switches::kDisableRendererBackgrounding);
}

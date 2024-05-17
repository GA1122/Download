  void RunTest(const std::string& file_name) {
    AutoAcceptInstall();
    ui_test_utils::NavigateToURL(browser(),
                                 GenerateTestServerUrl(kAppDomain, file_name));
    WebstoreInstallerTest::RunTest("runTest");
  }

void LiveSyncTest::TearDown() {
  InProcessBrowserTest::TearDown();

  TearDownLocalPythonTestServer();

  TearDownLocalTestServer();
}

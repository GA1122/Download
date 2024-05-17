void SyncTest::TearDown() {
  InProcessBrowserTest::TearDown();

  TearDownLocalPythonTestServer();

  TearDownLocalTestServer();
}

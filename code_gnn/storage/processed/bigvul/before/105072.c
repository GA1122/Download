void LiveSyncTest::TearDownInProcessBrowserTestFixture() {
  mock_host_resolver_override_.reset();

  URLFetcher::set_factory(NULL);
}

void SearchProviderTest::TearDown() {
  message_loop_.RunAllPending();

  URLFetcher::set_factory(NULL);

  provider_ = NULL;
}

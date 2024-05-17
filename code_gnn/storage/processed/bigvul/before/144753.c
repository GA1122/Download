  LocalSiteCharacteristicsDatabaseTest()
      : scoped_set_tick_clock_for_testing_(&test_clock_),
        test_server_(net::test_server::EmbeddedTestServer::TYPE_HTTPS) {}

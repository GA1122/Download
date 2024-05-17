  void CacheUrl(const GURL& url) {
    std::unique_ptr<network::ResourceRequest> request =
        std::make_unique<network::ResourceRequest>();
    request->url = url;
    content::SimpleURLLoaderTestHelper simple_loader_helper;
    url::Origin origin = url::Origin::Create(url);
    request->trusted_params = network::ResourceRequest::TrustedParams();
    request->trusted_params->network_isolation_key =
        net::NetworkIsolationKey(origin, origin);
    std::unique_ptr<network::SimpleURLLoader> simple_loader =
        network::SimpleURLLoader::Create(std::move(request),
                                         TRAFFIC_ANNOTATION_FOR_TESTS);
    simple_loader->DownloadToStringOfUnboundedSizeUntilCrashAndDie(
        browser()->profile()->GetURLLoaderFactory().get(),
        simple_loader_helper.GetCallback());
    simple_loader_helper.WaitForCallback();
    ASSERT_TRUE(simple_loader_helper.response_body());
    if (url.IntPort() == embedded_test_server()->port()) {
      EXPECT_EQ(1u, connection_tracker()->GetAcceptedSocketCount());
      EXPECT_EQ(1u, connection_tracker()->GetReadSocketCount());
    } else {
      EXPECT_EQ(url.IntPort(), preconnecting_test_server_.port());
      EXPECT_EQ(
          1u,
          preconnecting_server_connection_tracker()->GetAcceptedSocketCount());
      EXPECT_EQ(
          1u, preconnecting_server_connection_tracker()->GetReadSocketCount());
    }
    ResetNetworkState();
  }

void ResourcePrefetchPredictorTest::InitializeSampleData() {
  {   
    RedirectData bbc = CreateRedirectData("bbc.com", 9);
    InitializeRedirectStat(bbc.add_redirect_endpoints(),
                           GURL("https://www.bbc.com"), 8, 4, 1);
    InitializeRedirectStat(bbc.add_redirect_endpoints(),
                           GURL("https://m.bbc.com"), 5, 8, 0);
    InitializeRedirectStat(bbc.add_redirect_endpoints(),
                           GURL("http://bbc.co.uk"), 1, 3, 0);
    InitializeRedirectStat(bbc.add_redirect_endpoints(),
                           GURL("https://bbc.co.uk"), 1, 3, 0);

    RedirectData microsoft = CreateRedirectData("microsoft.com", 10);
    InitializeRedirectStat(microsoft.add_redirect_endpoints(),
                           GURL("https://www.microsoft.com"), 10, 0, 0);

    test_host_redirect_data_.clear();
    test_host_redirect_data_.insert(std::make_pair(bbc.primary_key(), bbc));
    test_host_redirect_data_.insert(
        std::make_pair(microsoft.primary_key(), microsoft));
  }

  {   
    OriginData google = CreateOriginData("google.com", 12);
    InitializeOriginStat(google.add_origins(), "https://static.google.com", 12,
                         0, 0, 3., false, true);
    InitializeOriginStat(google.add_origins(), "https://cats.google.com", 12, 0,
                         0, 5., true, true);
    test_origin_data_.insert({"google.com", google});

    OriginData twitter = CreateOriginData("twitter.com", 42);
    InitializeOriginStat(twitter.add_origins(), "https://static.twitter.com",
                         12, 0, 0, 3., false, true);
    InitializeOriginStat(twitter.add_origins(), "https://random.140chars.com",
                         12, 0, 0, 3., false, true);
    test_origin_data_.insert({"twitter.com", twitter});
  }
}

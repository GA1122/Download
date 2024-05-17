  static void CheckAllowedDomainsHeader(
      const std::string& allowed_domain,
      const std::map<GURL, net::HttpRequestHeaders>& urls_requested,
      const GURL& url) {
    auto iter = urls_requested.find(url);
    ASSERT_TRUE(iter != urls_requested.end());
    if (allowed_domain.empty()) {
      EXPECT_TRUE(
          !iter->second.HasHeader(safe_search_util::kGoogleAppsAllowedDomains));
      return;
    }

    std::string header;
    iter->second.GetHeader(safe_search_util::kGoogleAppsAllowedDomains,
                           &header);
    EXPECT_EQ(header, allowed_domain);
  }

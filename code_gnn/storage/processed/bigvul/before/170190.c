  static void CheckYouTubeRestricted(
      int youtube_restrict_mode,
      const std::map<GURL, net::HttpRequestHeaders>& urls_requested,
      const GURL& url) {
    auto iter = urls_requested.find(url);
    ASSERT_TRUE(iter != urls_requested.end());
    std::string header;
    iter->second.GetHeader(safe_search_util::kYouTubeRestrictHeaderName,
                           &header);
    if (youtube_restrict_mode == safe_search_util::YOUTUBE_RESTRICT_OFF) {
      EXPECT_TRUE(header.empty());
    } else if (youtube_restrict_mode ==
               safe_search_util::YOUTUBE_RESTRICT_MODERATE) {
      EXPECT_EQ(header, safe_search_util::kYouTubeRestrictHeaderValueModerate);
    } else if (youtube_restrict_mode ==
               safe_search_util::YOUTUBE_RESTRICT_STRICT) {
      EXPECT_EQ(header, safe_search_util::kYouTubeRestrictHeaderValueStrict);
    }
  }

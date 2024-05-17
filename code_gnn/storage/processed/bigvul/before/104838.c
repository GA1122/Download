  void PrerenderTestURL(
      const GURL& url,
      FinalStatus expected_final_status,
      int total_navigations) {
    ASSERT_TRUE(test_server()->Start());
    std::deque<FinalStatus> expected_final_status_queue(1,
                                                        expected_final_status);
    PrerenderTestURLImpl(url, expected_final_status_queue, total_navigations);
  }

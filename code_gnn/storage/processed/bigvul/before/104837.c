  void PrerenderTestURL(
      const std::string& html_file,
      const std::deque<FinalStatus>& expected_final_status_queue,
      int total_navigations) {
    ASSERT_TRUE(test_server()->Start());
    PrerenderTestURLImpl(test_server()->GetURL(html_file),
                         expected_final_status_queue,
                         total_navigations);
  }

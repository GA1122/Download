  void PrerenderTestURL(const std::string& html_file,
                        FinalStatus expected_final_status,
                        int total_navigations) {
    std::deque<FinalStatus> expected_final_status_queue(1,
                                                        expected_final_status);
    PrerenderTestURL(html_file,
                     expected_final_status_queue,
                     total_navigations);
  }

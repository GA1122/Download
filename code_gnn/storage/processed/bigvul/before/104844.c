  WaitForLoadPrerenderContentsFactory(
      int number_of_loads,
      const std::deque<FinalStatus>& expected_final_status_queue)
      : number_of_loads_(number_of_loads) {
    expected_final_status_queue_.resize(expected_final_status_queue.size());
    std::copy(expected_final_status_queue.begin(),
              expected_final_status_queue.end(),
              expected_final_status_queue_.begin());
    LOG(INFO) << "Factory created with queue length " <<
                 expected_final_status_queue_.size();
  }

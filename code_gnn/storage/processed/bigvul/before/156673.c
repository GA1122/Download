  void MaybeStartRequests() {
    for (auto it : num_remaining_requests_to_delay_for_path_) {
      if (it.second > 0)
        return;
    }
    for (const auto it : send_response_closures_) {
      it.Run();
    }
  }

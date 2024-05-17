  void OnBeforeNetworkStart(bool* defer) {
    *defer = defer_on_before_network_start_;
    observed_before_network_start_ = true;
  }

  void SetDelayedRequestsForPath(const std::string& path, int num_delayed) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    DCHECK(!test_server_->Started());
    num_remaining_requests_to_delay_for_path_[path] = num_delayed;
  }

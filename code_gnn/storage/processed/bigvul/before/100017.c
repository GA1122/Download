  virtual void didReceiveData(
      WebURLLoader*, const char* data, int data_size) {
    resource_client_->DidReceiveData(
        data, data_size, byte_range_lower_bound_);
  }

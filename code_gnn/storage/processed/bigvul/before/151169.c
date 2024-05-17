  virtual void DidReceiveDataForClient(const char* data, unsigned data_length) {
    if (!data_length)
      return;
    raw_data_->Append(data, data_length);
  }

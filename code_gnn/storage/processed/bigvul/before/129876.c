  BackgroundTracingManagerUploadConfigWrapper(const base::Closure& callback)
      : callback_(callback), receive_count_(0) {
    receive_callback_ =
        base::Bind(&BackgroundTracingManagerUploadConfigWrapper::Upload,
                   base::Unretained(this));
  }

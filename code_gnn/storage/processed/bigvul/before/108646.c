  URLRequestTest() : default_context_(true) {
    default_context_.set_network_delegate(&default_network_delegate_);
    default_context_.Init();
  }

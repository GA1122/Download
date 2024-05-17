  ~TestInterceptor() {
    URLRequest::Deprecated::UnregisterRequestInterceptor(this);
  }

  void ServeRequests() {
    Platform::Current()->GetURLLoaderMockFactory()->ServeAsynchronousRequests();
  }

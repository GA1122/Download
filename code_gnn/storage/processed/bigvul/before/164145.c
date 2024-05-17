  void ResetStorage() {
    service_->storage_.reset(new MockAppCacheStorage(service_.get()));
  }

  void DecrementRefCount(const std::string& uuid) {
    context_->DecrementBlobRefCount(uuid);
  }

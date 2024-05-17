  ~CallbackHandler() {
    if (services_)
      services_->Release();
  }

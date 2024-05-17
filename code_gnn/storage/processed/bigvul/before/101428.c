  SyncSession* MakeSession() {
    return new SyncSession(context_.get(), this, SyncSourceInfo(), routes_,
                           std::vector<ModelSafeWorker*>());
  }

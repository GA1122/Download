  explicit WriteSyncCallbackEntry(WriteSyncCallback cb)
      : callback(std::move(cb)), bytes(0) {}

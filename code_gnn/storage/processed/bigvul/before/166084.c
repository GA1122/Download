  explicit ReadDirectorySyncCallbackEntry(ReadDirectorySyncCallback cb)
      : callback(std::move(cb)) {}

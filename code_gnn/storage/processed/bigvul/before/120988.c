  explicit SelfDeletingDelegate(const CompletionCallback& callback)
      : socket_stream_(), callback_(callback) {}

  explicit WebSocketEventRecorder(net::CompletionCallback* callback)
      : onopen_(NULL),
        onmessage_(NULL),
        onclose_(NULL),
        callback_(callback) {}

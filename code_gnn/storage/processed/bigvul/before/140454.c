  FakeStreamRequest(RequestPriority priority,
                    HttpStreamRequest::Delegate* delegate)
      : priority_(priority),
        delegate_(delegate),
        websocket_stream_create_helper_(NULL) {}

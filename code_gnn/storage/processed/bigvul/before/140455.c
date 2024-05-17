  FakeStreamRequest(RequestPriority priority,
                    HttpStreamRequest::Delegate* delegate,
                    WebSocketHandshakeStreamBase::CreateHelper* create_helper)
      : priority_(priority),
        delegate_(delegate),
        websocket_stream_create_helper_(create_helper) {}

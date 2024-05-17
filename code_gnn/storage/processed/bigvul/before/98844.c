void WebSocketExperimentTask::DoLoop(int result) {
  if (next_state_ == STATE_NONE) {
    Finish(net::ERR_ABORTED);
    return;
  }
  do {
    State state = next_state_;
    next_state_ = STATE_NONE;
    switch (state) {
      case STATE_URL_FETCH:
        result = DoURLFetch();
        break;
      case STATE_URL_FETCH_COMPLETE:
        result = DoURLFetchComplete(result);
        break;
      case STATE_WEBSOCKET_CONNECT:
        result = DoWebSocketConnect();
        break;
      case STATE_WEBSOCKET_CONNECT_COMPLETE:
        result = DoWebSocketConnectComplete(result);
        break;
      case STATE_WEBSOCKET_SEND_HELLO:
        result = DoWebSocketSendHello();
        break;
      case STATE_WEBSOCKET_RECV_HELLO:
        result = DoWebSocketReceiveHello(result);
        break;
      case STATE_WEBSOCKET_KEEP_IDLE:
        result = DoWebSocketKeepIdle();
        break;
      case STATE_WEBSOCKET_KEEP_IDLE_COMPLETE:
        result = DoWebSocketKeepIdleComplete(result);
        break;
      case STATE_WEBSOCKET_RECV_PUSH_MESSAGE:
        result = DoWebSocketReceivePushMessage(result);
        break;
      case STATE_WEBSOCKET_ECHO_BACK_MESSAGE:
        result = DoWebSocketEchoBackMessage();
        break;
      case STATE_WEBSOCKET_RECV_BYE:
        result = DoWebSocketReceiveBye(result);
        break;
      case STATE_WEBSOCKET_CLOSE:
        result = DoWebSocketClose();
        break;
      case STATE_WEBSOCKET_CLOSE_COMPLETE:
        result = DoWebSocketCloseComplete(result);
        break;
      default:
        NOTREACHED();
        break;
    }
    result_.last_state = state;
  } while (result != net::ERR_IO_PENDING && next_state_ != STATE_NONE);

  if (result != net::ERR_IO_PENDING)
    Finish(result);
}

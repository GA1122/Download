QuicClientPromisedInfo::QuicClientPromisedInfo(
    QuicSpdyClientSessionBase* session,
    QuicStreamId id,
    string url)
    : session_(session),
      id_(id),
      url_(std::move(url)),
      client_request_delegate_(nullptr) {}

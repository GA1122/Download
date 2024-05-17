MockNetworkTransaction::MockNetworkTransaction(RequestPriority priority,
                                               MockNetworkLayer* factory)
    : request_(nullptr),
      data_cursor_(0),
      content_length_(0),
      priority_(priority),
      read_handler_(nullptr),
      websocket_handshake_stream_create_helper_(nullptr),
      transaction_factory_(factory->AsWeakPtr()),
      received_bytes_(0),
      sent_bytes_(0),
      socket_log_id_(NetLog::Source::kInvalidId),
      done_reading_called_(false),
      weak_factory_(this) {}

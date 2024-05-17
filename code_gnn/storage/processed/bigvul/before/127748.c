MockNetworkLayer::MockNetworkLayer()
    : transaction_count_(0),
      done_reading_called_(false),
      stop_caching_called_(false),
      last_create_transaction_priority_(DEFAULT_PRIORITY),
      clock_(nullptr) {
}

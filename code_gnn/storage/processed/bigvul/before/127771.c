void MockNetworkLayer::TransactionDoneReading() {
  CHECK(!done_reading_called_);
  done_reading_called_ = true;
}

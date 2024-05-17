int MockNetworkLayer::CreateTransaction(
    RequestPriority priority,
    std::unique_ptr<HttpTransaction>* trans) {
  transaction_count_++;
  last_create_transaction_priority_ = priority;
  std::unique_ptr<MockNetworkTransaction> mock_transaction(
      new MockNetworkTransaction(priority, this));
  last_transaction_ = mock_transaction->AsWeakPtr();
  *trans = std::move(mock_transaction);
  return OK;
}

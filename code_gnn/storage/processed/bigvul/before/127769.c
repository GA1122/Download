void MockNetworkTransaction::StopCaching() {
  if (transaction_factory_.get())
    transaction_factory_->TransactionStopCaching();
}

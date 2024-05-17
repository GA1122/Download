void MockNetworkTransaction::DoneReading() {
  CHECK(!done_reading_called_);
  done_reading_called_ = true;
  if (transaction_factory_.get())
    transaction_factory_->TransactionDoneReading();
}

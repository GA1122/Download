void NaClIPCAdapter::ClearToBeSent() {
  lock_.AssertAcquired();

  std::string empty;
  locked_data_.to_be_sent_.swap(empty);
}

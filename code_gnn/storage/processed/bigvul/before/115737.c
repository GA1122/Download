 void ConnectionToClient::UpdateSequenceNumber(int64 sequence_number) {
  handler_->OnSequenceNumberUpdated(this, sequence_number);
}

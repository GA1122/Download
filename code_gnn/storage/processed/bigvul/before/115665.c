 void ClientSession::OnSequenceNumberUpdated(
    protocol::ConnectionToClient* connection, int64 sequence_number) {
  DCHECK_EQ(connection_.get(), connection);
  event_handler_->OnSessionSequenceNumber(this, sequence_number);
 }

void P2PQuicTransportImpl::Stop() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  if (IsClosed()) {
    return;
  }
  connection_->CloseConnection(
      quic::QuicErrorCode::QUIC_CONNECTION_CANCELLED, kClosingDetails,
      quic::ConnectionCloseBehavior::SEND_CONNECTION_CLOSE_PACKET);
}

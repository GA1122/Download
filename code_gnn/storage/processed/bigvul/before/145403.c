void P2PQuicTransportImpl::OnConnectionClosed(
    quic::QuicErrorCode error,
    const std::string& error_details,
    quic::ConnectionCloseSource source) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  quic::QuicSession::OnConnectionClosed(error, error_details, source);
  if (error != quic::QuicErrorCode::QUIC_CONNECTION_CANCELLED) {
    delegate_->OnConnectionFailed(
        error_details, source == quic::ConnectionCloseSource::FROM_PEER);
  } else if (source == quic::ConnectionCloseSource::FROM_PEER) {
    delegate_->OnRemoteStopped();
  }
}

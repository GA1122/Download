void P2PQuicTransportImpl::OnCryptoHandshakeEvent(CryptoHandshakeEvent event) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  QuicSession::OnCryptoHandshakeEvent(event);
  if (event == HANDSHAKE_CONFIRMED) {
    DCHECK(IsEncryptionEstablished());
    DCHECK(IsCryptoHandshakeConfirmed());
    delegate_->OnConnected();
  }
}

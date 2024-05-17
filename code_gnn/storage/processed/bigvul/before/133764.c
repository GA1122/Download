bool SSLClientSocketOpenSSL::GetSSLInfo(SSLInfo* ssl_info) {
  ssl_info->Reset();
  if (!server_cert_.get())
    return false;

  ssl_info->cert = server_cert_verify_result_.verified_cert;
  ssl_info->cert_status = server_cert_verify_result_.cert_status;
  ssl_info->is_issued_by_known_root =
      server_cert_verify_result_.is_issued_by_known_root;
  ssl_info->public_key_hashes =
    server_cert_verify_result_.public_key_hashes;
  ssl_info->client_cert_sent =
      ssl_config_.send_client_cert && ssl_config_.client_cert.get();
  ssl_info->channel_id_sent = WasChannelIDSent();

  RecordChannelIDSupport(server_bound_cert_service_,
                         channel_id_xtn_negotiated_,
                         ssl_config_.channel_id_enabled,
                         crypto::ECPrivateKey::IsSupported());

  const SSL_CIPHER* cipher = SSL_get_current_cipher(ssl_);
  CHECK(cipher);
  ssl_info->security_bits = SSL_CIPHER_get_bits(cipher, NULL);
  const COMP_METHOD* compression = SSL_get_current_compression(ssl_);

  ssl_info->connection_status = EncodeSSLConnectionStatus(
      SSL_CIPHER_get_id(cipher),
      compression ? compression->type : 0,
      GetNetSSLVersion(ssl_));

  bool peer_supports_renego_ext = !!SSL_get_secure_renegotiation_support(ssl_);
  if (!peer_supports_renego_ext)
    ssl_info->connection_status |= SSL_CONNECTION_NO_RENEGOTIATION_EXTENSION;
  UMA_HISTOGRAM_ENUMERATION("Net.RenegotiationExtensionSupported",
                            implicit_cast<int>(peer_supports_renego_ext), 2);

  if (ssl_config_.version_fallback)
    ssl_info->connection_status |= SSL_CONNECTION_VERSION_FALLBACK;

  ssl_info->handshake_type = SSL_session_reused(ssl_) ?
      SSLInfo::HANDSHAKE_RESUME : SSLInfo::HANDSHAKE_FULL;

  DVLOG(3) << "Encoded connection status: cipher suite = "
      << SSLConnectionStatusToCipherSuite(ssl_info->connection_status)
      << " version = "
      << SSLConnectionStatusToVersion(ssl_info->connection_status);
  return true;
}

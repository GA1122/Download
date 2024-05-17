void SSLClientSocketOpenSSL::ChannelIDRequestCallback(SSL* ssl,
                                                      EVP_PKEY** pkey) {
  DVLOG(3) << "OpenSSL ChannelIDRequestCallback called";
  DCHECK_EQ(ssl, ssl_);
  DCHECK(!*pkey);

  channel_id_xtn_negotiated_ = true;
  if (!channel_id_private_key_.size()) {
    channel_id_request_return_value_ =
        server_bound_cert_service_->GetOrCreateDomainBoundCert(
            host_and_port_.host(),
            &channel_id_private_key_,
            &channel_id_cert_,
            base::Bind(&SSLClientSocketOpenSSL::OnHandshakeIOComplete,
                       base::Unretained(this)),
            &channel_id_request_handle_);
    if (channel_id_request_return_value_ != OK)
      return;
  }

  std::vector<uint8> encrypted_private_key_info;
  std::vector<uint8> subject_public_key_info;
  encrypted_private_key_info.assign(
      channel_id_private_key_.data(),
      channel_id_private_key_.data() + channel_id_private_key_.size());
  subject_public_key_info.assign(
      channel_id_cert_.data(),
      channel_id_cert_.data() + channel_id_cert_.size());
  scoped_ptr<crypto::ECPrivateKey> ec_private_key(
      crypto::ECPrivateKey::CreateFromEncryptedPrivateKeyInfo(
          ServerBoundCertService::kEPKIPassword,
          encrypted_private_key_info,
          subject_public_key_info));
  if (!ec_private_key)
    return;
  set_channel_id_sent(true);
   *pkey = EVP_PKEY_dup(ec_private_key->key());
 }

bool SSLClientSocketOpenSSL::Init() {
  DCHECK(!ssl_);
  DCHECK(!transport_bio_);

  SSLContext* context = SSLContext::GetInstance();
  crypto::OpenSSLErrStackTracer err_tracer(FROM_HERE);

  ssl_ = SSL_new(context->ssl_ctx());
  if (!ssl_ || !context->SetClientSocketForSSL(ssl_, this))
    return false;

  if (!SSL_set_tlsext_host_name(ssl_, host_and_port_.host().c_str()))
    return false;

  trying_cached_session_ = context->session_cache()->SetSSLSessionWithKey(
      ssl_, GetSocketSessionCacheKey(*this));

  BIO* ssl_bio = NULL;
  if (!BIO_new_bio_pair(&ssl_bio, 0, &transport_bio_, 0))
    return false;
  DCHECK(ssl_bio);
  DCHECK(transport_bio_);

  SSL_set_bio(ssl_, ssl_bio, ssl_bio);

  SslSetClearMask options;
  options.ConfigureFlag(SSL_OP_NO_SSLv2, true);
  bool ssl3_enabled = (ssl_config_.version_min == SSL_PROTOCOL_VERSION_SSL3);
  options.ConfigureFlag(SSL_OP_NO_SSLv3, !ssl3_enabled);
  bool tls1_enabled = (ssl_config_.version_min <= SSL_PROTOCOL_VERSION_TLS1 &&
                       ssl_config_.version_max >= SSL_PROTOCOL_VERSION_TLS1);
  options.ConfigureFlag(SSL_OP_NO_TLSv1, !tls1_enabled);
#if defined(SSL_OP_NO_TLSv1_1)
  bool tls1_1_enabled =
      (ssl_config_.version_min <= SSL_PROTOCOL_VERSION_TLS1_1 &&
       ssl_config_.version_max >= SSL_PROTOCOL_VERSION_TLS1_1);
  options.ConfigureFlag(SSL_OP_NO_TLSv1_1, !tls1_1_enabled);
#endif
#if defined(SSL_OP_NO_TLSv1_2)
  bool tls1_2_enabled =
      (ssl_config_.version_min <= SSL_PROTOCOL_VERSION_TLS1_2 &&
       ssl_config_.version_max >= SSL_PROTOCOL_VERSION_TLS1_2);
  options.ConfigureFlag(SSL_OP_NO_TLSv1_2, !tls1_2_enabled);
#endif

#if defined(SSL_OP_NO_COMPRESSION)
  options.ConfigureFlag(SSL_OP_NO_COMPRESSION, true);
#endif

  options.ConfigureFlag(SSL_OP_LEGACY_SERVER_CONNECT, true);

  SSL_set_options(ssl_, options.set_mask);
  SSL_clear_options(ssl_, options.clear_mask);

  SslSetClearMask mode;

#if defined(SSL_MODE_RELEASE_BUFFERS)
  mode.ConfigureFlag(SSL_MODE_RELEASE_BUFFERS, true);
#endif

#if defined(SSL_MODE_SMALL_BUFFERS)
  mode.ConfigureFlag(SSL_MODE_SMALL_BUFFERS, true);
#endif

  SSL_set_mode(ssl_, mode.set_mask);
  SSL_clear_mode(ssl_, mode.clear_mask);

  STACK_OF(SSL_CIPHER)* ciphers = SSL_get_ciphers(ssl_);
  DCHECK(ciphers);
  std::string command("DEFAULT:!NULL:!aNULL:!IDEA:!FZA:!SRP:!SHA256:!SHA384:"
                      "!aECDH:!AESGCM+AES256");
  for (int i = 0; i < sk_SSL_CIPHER_num(ciphers); ++i) {
    const SSL_CIPHER* cipher = sk_SSL_CIPHER_value(ciphers, i);
    const uint16 id = SSL_CIPHER_get_id(cipher);
    bool disable = SSL_CIPHER_get_bits(cipher, NULL) < 80;
    if (!disable) {
      disable = std::find(ssl_config_.disabled_cipher_suites.begin(),
                          ssl_config_.disabled_cipher_suites.end(), id) !=
                    ssl_config_.disabled_cipher_suites.end();
    }
    if (disable) {
       const char* name = SSL_CIPHER_get_name(cipher);
       DVLOG(3) << "Found cipher to remove: '" << name << "', ID: " << id
                << " strength: " << SSL_CIPHER_get_bits(cipher, NULL);
       command.append(":!");
       command.append(name);
     }
  }
  int rv = SSL_set_cipher_list(ssl_, command.c_str());
  LOG_IF(WARNING, rv != 1) << "SSL_set_cipher_list('" << command << "') "
                              "returned " << rv;

  if (IsChannelIDEnabled(ssl_config_, server_bound_cert_service_)) {
    SSL_enable_tls_channel_id(ssl_);
  }

  return true;
}

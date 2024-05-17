int SSLClientSocketOpenSSL::ClientCertRequestCallback(SSL* ssl,
                                                      X509** x509,
                                                      EVP_PKEY** pkey) {
  DVLOG(3) << "OpenSSL ClientCertRequestCallback called";
  DCHECK(ssl == ssl_);
  DCHECK(*x509 == NULL);
  DCHECK(*pkey == NULL);

  if (!ssl_config_.send_client_cert) {
    client_auth_cert_needed_ = true;
    STACK_OF(X509_NAME) *authorities = SSL_get_client_CA_list(ssl);
    for (int i = 0; i < sk_X509_NAME_num(authorities); i++) {
      X509_NAME *ca_name = (X509_NAME *)sk_X509_NAME_value(authorities, i);
      unsigned char* str = NULL;
      int length = i2d_X509_NAME(ca_name, &str);
      cert_authorities_.push_back(std::string(
          reinterpret_cast<const char*>(str),
          static_cast<size_t>(length)));
      OPENSSL_free(str);
    }

    return -1;   
  }

  if (ssl_config_.client_cert.get()) {
    OpenSSLClientKeyStore::ScopedEVP_PKEY privkey;
    if (OpenSSLClientKeyStore::GetInstance()->FetchClientCertPrivateKey(
            ssl_config_.client_cert.get(), &privkey)) {
      *x509 = X509Certificate::DupOSCertHandle(
          ssl_config_.client_cert->os_cert_handle());
      *pkey = privkey.release();
      return 1;
    }
    LOG(WARNING) << "Client cert found without private key";
  }

  return 0;
}

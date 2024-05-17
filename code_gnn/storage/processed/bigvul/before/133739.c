  static int ClientCertCallback(SSL* ssl, X509** x509, EVP_PKEY** pkey) {
    SSLClientSocketOpenSSL* socket = GetInstance()->GetClientSocketFromSSL(ssl);
    CHECK(socket);
    return socket->ClientCertRequestCallback(ssl, x509, pkey);
  }

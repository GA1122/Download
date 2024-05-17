  static void ChannelIDCallback(SSL* ssl, EVP_PKEY** pkey) {
    SSLClientSocketOpenSSL* socket = GetInstance()->GetClientSocketFromSSL(ssl);
    CHECK(socket);
     socket->ChannelIDRequestCallback(ssl, pkey);
   }

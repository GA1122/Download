   static int SelectNextProtoCallback(SSL* ssl,
                                      unsigned char** out, unsigned char* outlen,
                                      const unsigned char* in,
                                     unsigned int inlen, void* arg) {
    SSLClientSocketOpenSSL* socket = GetInstance()->GetClientSocketFromSSL(ssl);
    return socket->SelectNextProtoCallback(out, outlen, in, inlen);
  }

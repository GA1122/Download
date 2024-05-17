  static std::string GetSessionCacheKey(const SSL* ssl) {
    SSLClientSocketOpenSSL* socket = GetInstance()->GetClientSocketFromSSL(ssl);
    DCHECK(socket);
    return GetSocketSessionCacheKey(*socket);
  }

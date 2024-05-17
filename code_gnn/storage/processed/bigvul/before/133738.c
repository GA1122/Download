void SSLClientSocket::ClearSessionCache() {
  SSLClientSocketOpenSSL::SSLContext* context =
      SSLClientSocketOpenSSL::SSLContext::GetInstance();
  context->session_cache()->Flush();
  OpenSSLClientKeyStore::GetInstance()->Flush();
}

int SetSSLCipherSuite(int connection_status, int cipher_suite) {
  connection_status &= ~net::SSL_CONNECTION_CIPHERSUITE_MASK;
  return cipher_suite | connection_status;
}

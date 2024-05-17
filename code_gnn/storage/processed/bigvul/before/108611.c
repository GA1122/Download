void CheckSSLInfo(const SSLInfo& ssl_info) {
  if (ssl_info.cert.get() &&
      ssl_info.cert.get()->issuer().GetDisplayName() == "Chrome Internal") {
    EXPECT_EQ(ssl_info.security_bits, -1);
    return;
  }

  EXPECT_GT(ssl_info.security_bits, 0);

  int cipher_suite = SSLConnectionStatusToCipherSuite(
      ssl_info.connection_status);
  EXPECT_NE(0, cipher_suite);
}

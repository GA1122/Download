  void StartServerWithExpiredCert() {
    expired_https_server_.reset(
        new net::EmbeddedTestServer(net::EmbeddedTestServer::TYPE_HTTPS));
    expired_https_server_->SetSSLConfig(net::EmbeddedTestServer::CERT_EXPIRED);
    expired_https_server_->AddDefaultHandlers(
        base::FilePath(FILE_PATH_LITERAL("chrome/test/data")));
    ASSERT_TRUE(expired_https_server_->Start());
  }

  void DoConnection(const TestServer::SSLOptions& ssl_options,
                    CertStatus* out_cert_status) {
    TestServer test_server(TestServer::TYPE_HTTPS,
                           ssl_options,
                           FilePath(FILE_PATH_LITERAL("net/data/ssl")));
    ASSERT_TRUE(test_server.Start());

    TestDelegate d;
    d.set_allow_certificate_errors(true);
    URLRequest r(test_server.GetURL(""), &d, &context_);
    r.Start();

    MessageLoop::current()->Run();

    EXPECT_EQ(1, d.response_started_count());
    *out_cert_status = r.ssl_info().cert_status;
  }

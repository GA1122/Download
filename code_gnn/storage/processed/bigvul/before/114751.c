void PPAPITestBase::RunTestWithSSLServer(const std::string& test_case) {
  FilePath document_root;
  ASSERT_TRUE(GetHTTPDocumentRoot(&document_root));
  net::TestServer test_server(net::BaseTestServer::HTTPSOptions(),
                              document_root);
  ASSERT_TRUE(test_server.Start());
  uint16_t port = test_server.host_port_pair().port();
  RunHTTPTestServer(document_root, test_case,
                    StringPrintf("ssl_server_port=%d", port));
}

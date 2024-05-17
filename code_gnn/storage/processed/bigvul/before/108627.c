  explicit LocalHttpTestServer(const FilePath& document_root)
      : TestServer(TestServer::TYPE_HTTP,
                   ScopedCustomUrlRequestTestHttpHost::value(),
                   document_root) {}

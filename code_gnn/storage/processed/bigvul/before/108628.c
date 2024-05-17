  LocalHttpTestServer()
      : TestServer(TestServer::TYPE_HTTP,
                   ScopedCustomUrlRequestTestHttpHost::value(),
                    FilePath()) {}

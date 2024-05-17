  DevtoolsInterceptionWithAuthProxyTest()
      : proxy_server_(net::SpawnedTestServer::TYPE_BASIC_AUTH_PROXY,
                      base::FilePath(FILE_PATH_LITERAL("headless/test/data"))) {
  }

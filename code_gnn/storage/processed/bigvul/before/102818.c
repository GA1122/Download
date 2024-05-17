  SSLUITest()
      : https_server_(
            HTTPSOptions(HTTPSOptions::CERT_OK), FilePath(kDocRoot)),
        https_server_expired_(
            HTTPSOptions(HTTPSOptions::CERT_EXPIRED), FilePath(kDocRoot)),
        https_server_mismatched_(
            HTTPSOptions(HTTPSOptions::CERT_MISMATCHED_NAME),
            FilePath(kDocRoot)) {
    EnableDOMAutomation();
  }

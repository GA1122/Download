  DataReductionProxyWarmupURLBrowsertest()
      : via_header_(std::get<1>(GetParam()) ? "1.1 Chrome-Compression-Proxy"
                                            : "bad"),
        primary_server_(GetTestServerType()),
        secondary_server_(GetTestServerType()) {}

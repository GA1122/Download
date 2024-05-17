  std::string GetHistogramName() {
    return base::StrCat(
        {"DataReductionProxy.WarmupURLFetcherCallback.SuccessfulFetch.",
         std::get<0>(GetParam()) == ProxyServer_ProxyScheme_HTTP ? "Insecure"
                                                                 : "Secure",
         "Proxy.Core"});
  }

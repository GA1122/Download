  virtual ~SystemURLRequestContext() {
#if defined(USE_NSS) || defined(OS_IOS)
    net::SetURLRequestContextForNSSHttpIO(NULL);
#endif
  }

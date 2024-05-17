  ~HTTPSOCSPTest() {
#if defined(USE_NSS)
    ShutdownNSSHttpIO();
#endif
  }

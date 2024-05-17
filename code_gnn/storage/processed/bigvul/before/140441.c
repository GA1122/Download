CaptureGroupNameHttpProxySocketPool::CaptureGroupNameSocketPool(
    HostResolver* host_resolver,
    CertVerifier*  )
    : HttpProxyClientSocketPool(
          0, 0, NULL, host_resolver, NULL, NULL, NULL, NULL) {}

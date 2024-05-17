void CancelRequestAndPause(ProxyResolverV8Tracing* resolver,
                           ProxyResolver::RequestHandle request) {
  resolver->CancelRequest(request);

  base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(30));
}

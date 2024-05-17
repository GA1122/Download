void SyncTest::SetProxyConfig(net::URLRequestContextGetter* context_getter,
                              const net::ProxyConfig& proxy_config) {
  base::WaitableEvent done(false, false);
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&SetProxyConfigCallback, &done,
                 make_scoped_refptr(context_getter), proxy_config));
  done.Wait();
}

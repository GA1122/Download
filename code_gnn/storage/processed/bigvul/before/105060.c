void LiveSyncTest::SetProxyConfig(net::URLRequestContextGetter* context_getter,
                                  const net::ProxyConfig& proxy_config) {
  base::WaitableEvent done(false, false);
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      new SetProxyConfigTask(&done,
                             context_getter,
                             proxy_config));
  done.Wait();
}

  void WaitUntilProxyLookedUp(const GURL& url) {
    wait_event_ = WaitEvent::kProxy;
    DCHECK(waiting_on_proxy_.is_empty());
    waiting_on_proxy_ = url;
    Wait();
  }

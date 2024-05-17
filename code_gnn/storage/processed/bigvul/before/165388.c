StoragePartitionImpl::GetCookieManagerForBrowserProcess() {
  if (!cookie_manager_for_browser_process_ ||
      cookie_manager_for_browser_process_.encountered_error()) {
    GetNetworkContext()->GetCookieManager(
        mojo::MakeRequest(&cookie_manager_for_browser_process_));
  }
  return cookie_manager_for_browser_process_.get();
}

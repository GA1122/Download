void AppCacheDispatcherHost::OnGetResourceList(
    int host_id, std::vector<AppCacheResourceInfo>* params) {
  if (appcache_service_.get())
    backend_impl_.GetResourceList(host_id, params);
}

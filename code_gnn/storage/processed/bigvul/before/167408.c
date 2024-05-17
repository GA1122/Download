bool DownloadCoreServiceImpl::IsShelfEnabled() {
#if defined(OS_ANDROID)
  return true;
#else
  return !extension_event_router_ || extension_event_router_->IsShelfEnabled();
#endif
}

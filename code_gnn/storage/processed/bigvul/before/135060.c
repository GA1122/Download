void AppCacheHost::CompleteTransfer(int host_id, AppCacheFrontend* frontend) {
  host_id_ = host_id;
  frontend_ = frontend;
}

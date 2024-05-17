JsBackend* SyncBackendHost::GetJsBackend() {
  if (syncapi_initialized_) {
    return core_.get();
  } else {
    NOTREACHED();
    return NULL;
  }
}

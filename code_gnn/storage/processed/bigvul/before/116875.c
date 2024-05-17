TestWebKitPlatformSupport::createLocalStorageNamespace(
    const WebKit::WebString& path, unsigned quota) {
#ifdef ENABLE_NEW_DOM_STORAGE_BACKEND
  return dom_storage_system_.CreateLocalStorageNamespace();
#else
  return WebKit::WebStorageNamespace::createLocalStorageNamespace(path, quota);
#endif
}

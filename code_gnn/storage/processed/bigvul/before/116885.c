void TestWebKitPlatformSupport::dispatchStorageEvent(
    const WebKit::WebString& key,
    const WebKit::WebString& old_value, const WebKit::WebString& new_value,
    const WebKit::WebString& origin, const WebKit::WebURL& url,
    bool is_local_storage) {
#ifdef ENABLE_NEW_DOM_STORAGE_BACKEND
  NOTREACHED();
#endif
}

TestWebKitPlatformSupport::CreateWebSocketBridge(
    WebKit::WebSocketStreamHandle* handle,
    webkit_glue::WebSocketStreamHandleDelegate* delegate) {
  return SimpleSocketStreamBridge::Create(handle, delegate);
}

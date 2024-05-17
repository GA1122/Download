LayoutTestContentRendererClient::OverrideCreateWebMediaStreamCenter(
    WebMediaStreamCenterClient* client) {
#if defined(ENABLE_WEBRTC)
  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  return interfaces->CreateMediaStreamCenter(client);
#else
  return NULL;
#endif
}

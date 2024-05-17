LayoutTestContentRendererClient::OverrideCreateAudioDevice(
    double sample_rate) {
  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  return interfaces->CreateAudioDevice(sample_rate);
}

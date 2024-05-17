void RenderThread::OnNetworkStateChanged(bool online) {
  EnsureWebKitInitialized();
  WebNetworkStateNotifier::setOnLine(online);
}

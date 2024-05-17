bool ChromeContentRendererClient::IsAdblockPlusInstalled() {
  return extension_dispatcher_->extensions()->Contains(
      "cfhdojbkjhnklbpkdaibdccddilifddb");
}

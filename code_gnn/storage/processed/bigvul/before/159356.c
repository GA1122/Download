ChromeExtensionsAPIClient::GetMediaPerceptionAPIDelegate() {
  if (!media_perception_api_delegate_) {
    media_perception_api_delegate_ =
        std::make_unique<MediaPerceptionAPIDelegateChromeOS>();
  }
  return media_perception_api_delegate_.get();
}

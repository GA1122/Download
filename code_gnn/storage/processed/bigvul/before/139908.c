bool MockContentSettingsClient::allowImage(bool enabled_per_settings,
                                           const blink::WebURL& image_url) {
  bool allowed = enabled_per_settings && flags_->images_allowed();
  if (flags_->dump_web_content_settings_client_callbacks() && delegate_) {
    delegate_->PrintMessage(
        std::string("MockContentSettingsClient: allowImage(") +
        NormalizeLayoutTestURL(image_url.string().utf8()) +
        "): " + (allowed ? "true" : "false") + "\n");
  }
  return allowed;
}

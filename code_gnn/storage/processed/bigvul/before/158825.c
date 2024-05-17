blink::WebDisplayMode Browser::GetDisplayMode(
    const WebContents* web_contents) const {
  if (window_->IsFullscreen())
    return blink::kWebDisplayModeFullscreen;

  if (is_type_popup())
    return blink::kWebDisplayModeStandalone;

  return blink::kWebDisplayModeBrowser;
}

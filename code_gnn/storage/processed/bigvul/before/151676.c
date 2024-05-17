blink::WebDisplayMode Browser::GetDisplayMode(
    const WebContents* web_contents) const {
  if (window_->IsFullscreen())
    return blink::WebDisplayModeFullscreen;

  if (is_type_popup())
    return blink::WebDisplayModeStandalone;

  return blink::WebDisplayModeBrowser;
}

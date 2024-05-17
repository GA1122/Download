void EnableAccessibilityForWebContents(WebContents* web_contents) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  web_contents_impl->SetAccessibilityMode(ui::kAXModeComplete);
}

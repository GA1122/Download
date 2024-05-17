void WebContentsImpl::ForSecurityDropFullscreen() {
  WebContentsImpl* web_contents = this;
  while (web_contents) {
    if (web_contents->IsFullscreenForCurrentTab())
      web_contents->ExitFullscreen(true);
    web_contents = web_contents->GetOuterWebContents();
  }
}

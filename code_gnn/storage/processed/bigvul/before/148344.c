void WebContentsImpl::DidDisplayInsecureContent() {
  controller_.ssl_manager()->DidDisplayMixedContent();
}

void WebViewPlugin::didClearWindowObject(WebFrame* frame) {
  if (delegate_)
    delegate_->BindWebFrame(frame);
}

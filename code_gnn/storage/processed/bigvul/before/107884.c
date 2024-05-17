void WebViewPlugin::setToolTipText(const WebKit::WebString& text,
                                   WebKit::WebTextDirection hint) {
  if (container_)
    container_->element().setAttribute("title", text);
}

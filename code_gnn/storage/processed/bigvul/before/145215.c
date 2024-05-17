void Dispatcher::EnableCustomElementWhiteList() {
  blink::WebCustomElement::addEmbedderCustomElementName("appview");
  blink::WebCustomElement::addEmbedderCustomElementName("appviewbrowserplugin");
  blink::WebCustomElement::addEmbedderCustomElementName("extensionoptions");
  blink::WebCustomElement::addEmbedderCustomElementName(
      "extensionoptionsbrowserplugin");
  blink::WebCustomElement::addEmbedderCustomElementName("extensionview");
  blink::WebCustomElement::addEmbedderCustomElementName(
      "extensionviewbrowserplugin");
  blink::WebCustomElement::addEmbedderCustomElementName("webview");
  blink::WebCustomElement::addEmbedderCustomElementName("webviewbrowserplugin");
}

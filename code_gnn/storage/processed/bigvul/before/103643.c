void ChromeContentRendererClient::DidCreateScriptContext(WebFrame* frame) {
  EventBindings::HandleContextCreated(
      frame, false, extension_dispatcher_.get());
}

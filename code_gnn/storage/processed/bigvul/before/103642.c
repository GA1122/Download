void ChromeContentRendererClient::DidCreateIsolatedScriptContext(
  WebFrame* frame) {
  EventBindings::HandleContextCreated(frame, true, extension_dispatcher_.get());
}

void ChromeContentRendererClient::DidDestroyScriptContext(WebFrame* frame) {
  EventBindings::HandleContextDestroyed(frame);
}

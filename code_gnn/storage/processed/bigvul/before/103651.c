void ChromeContentRendererClient::SetExtensionDispatcher(
    ExtensionDispatcher* extension_dispatcher) {
  extension_dispatcher_.reset(extension_dispatcher);
}

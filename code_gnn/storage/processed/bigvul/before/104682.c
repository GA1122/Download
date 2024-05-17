v8::Extension* ExtensionProcessBindings::Get(
    ExtensionDispatcher* extension_dispatcher) {
  static v8::Extension* extension = new ExtensionImpl(extension_dispatcher);
  return extension;
}

void ExtensionDevToolsClientHost::InspectedTabClosing() {
  SendDetachedEvent();
  delete this;
}

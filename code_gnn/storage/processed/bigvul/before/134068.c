void ExtensionRegistry::Shutdown() {
  ClearAll();
  FOR_EACH_OBSERVER(ExtensionRegistryObserver, observers_, OnShutdown(this));
}

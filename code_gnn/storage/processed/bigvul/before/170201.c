  void DisableExtension(const std::string& id) {
    extensions::TestExtensionRegistryObserver observer(
        extensions::ExtensionRegistry::Get(browser()->profile()));
    extension_service()->DisableExtension(
        id, extensions::disable_reason::DISABLE_USER_ACTION);
    observer.WaitForExtensionUnloaded();
  }

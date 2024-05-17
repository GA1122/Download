void ExtensionBrowserTest::ReloadExtension(const std::string& extension_id) {
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile());
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  ASSERT_TRUE(extension);
  extensions::TestExtensionRegistryObserver observer(registry, extension_id);
  extensions::ExtensionSystem::Get(profile())
      ->extension_service()
      ->ReloadExtension(extension_id);
  observer.WaitForExtensionLoaded();

  base::RunLoop().RunUntilIdle();
  observer_->WaitForExtensionViewsToLoad();
}

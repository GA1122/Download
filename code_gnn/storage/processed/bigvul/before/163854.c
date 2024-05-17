void ExtensionBrowserTest::UninstallExtension(const std::string& extension_id) {
  ExtensionService* service = extensions::ExtensionSystem::Get(
      profile())->extension_service();
  service->UninstallExtension(extension_id,
                              extensions::UNINSTALL_REASON_FOR_TESTING,
                              base::Bind(&base::DoNothing),
                              NULL);
}

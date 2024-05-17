extensions::InstallTracker* AppListControllerDelegate::GetInstallTrackerFor(
    Profile* profile) {
  if (extensions::ExtensionSystem::Get(profile)->extension_service())
    return extensions::InstallTrackerFactory::GetForBrowserContext(profile);
  return NULL;
}

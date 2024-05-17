bool ChromeMetricsServiceClient::IsWebstoreExtension(base::StringPiece id) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  bool matched = false;
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  DCHECK(profile_manager);
  auto profiles = profile_manager->GetLoadedProfiles();
  for (Profile* profile : profiles) {
    DCHECK(profile);
    extensions::ExtensionRegistry* registry =
        extensions::ExtensionRegistry::Get(profile);
    if (!registry)
      continue;
    const extensions::Extension* extension = registry->GetExtensionById(
        id.as_string(), extensions::ExtensionRegistry::ENABLED);
    if (!extension)
      continue;
    if (!extension->from_webstore())
      return false;
    matched = true;
  }
  return matched;
#else
  return false;
#endif
}

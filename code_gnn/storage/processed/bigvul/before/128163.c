content::RenderViewHost* GetRenderViewHost() {
  const extensions::Extension* extension = FindCastExtension();
  if (!extension)
    return nullptr;
  Profile* profile = ProfileManager::GetActiveUserProfile();
  if (!profile)
    return nullptr;
  extensions::ProcessManager* pm = extensions::ProcessManager::Get(profile);
  return pm->GetBackgroundHostForExtension(extension->id())->render_view_host();
}

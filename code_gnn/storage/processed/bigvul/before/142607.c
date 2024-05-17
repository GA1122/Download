const Extension* TabHelper::GetExtension(const std::string& extension_app_id) {
  if (extension_app_id.empty())
    return NULL;

  content::BrowserContext* context = web_contents()->GetBrowserContext();
  return ExtensionRegistry::Get(context)->enabled_extensions().GetByID(
      extension_app_id);
}

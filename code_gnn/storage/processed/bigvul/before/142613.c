void TabHelper::OnExtensionUnloaded(
    content::BrowserContext* browser_context,
    const Extension* extension,
    UnloadedExtensionInfo::Reason reason) {
  DCHECK(extension_app_);
  if (extension == extension_app_)
    SetExtensionApp(nullptr);
}

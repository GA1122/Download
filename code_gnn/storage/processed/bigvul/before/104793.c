bool ExtensionService::ExtensionBindingsAllowed(const GURL& url) {
  if (GetExtensionByURL(url))
    return true;

  const Extension* extension = GetExtensionByWebExtent(url);
  return (extension && extension->location() == Extension::COMPONENT);
}

const extensions::Extension* GetNonBookmarkAppExtension(
    const ExtensionSet& extensions, const GURL& url) {
  const extensions::Extension* extension =
      extensions.GetExtensionOrAppByURL(url);
  if (extension && extension->from_bookmark())
    extension = NULL;
  return extension;
}

bool ShouldRemoveBookmarkOpenPagesUI(Profile* profile) {
#if defined(ENABLE_EXTENSIONS)
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile);
  if (!registry)
    return false;

  const extensions::ExtensionSet& extension_set =
      registry->enabled_extensions();

  for (extensions::ExtensionSet::const_iterator i = extension_set.begin();
       i != extension_set.end();
       ++i) {
    if (extensions::CommandService::RemovesBookmarkOpenPagesShortcut(i->get()))
      return true;
  }
#endif

  return false;
}

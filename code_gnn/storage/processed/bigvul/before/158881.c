  const Extension* GetExtension() {
    extensions::ExtensionRegistry* registry =
        extensions::ExtensionRegistry::Get(browser()->profile());
    for (const scoped_refptr<const extensions::Extension>& extension :
         registry->enabled_extensions()) {
      if (extension->name() == "App Test")
        return extension.get();
    }
    NOTREACHED();
    return NULL;
  }

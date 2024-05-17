const Extension* ExtensionApiTest::GetSingleLoadedExtension() {
  ExtensionRegistry* registry = ExtensionRegistry::Get(browser()->profile());

  const Extension* result = NULL;
  for (const scoped_refptr<const Extension>& extension :
       registry->enabled_extensions()) {
    if (extension->location() == Manifest::COMPONENT)
      continue;

    if (result != NULL) {
      message_ = base::StringPrintf(
          "Expected only one extension to be present.  Found %u.",
          static_cast<unsigned>(registry->enabled_extensions().size()));
      return NULL;
    }

    result = extension.get();
  }

  if (!result) {
    message_ = "extension pointer is NULL.";
    return NULL;
  }
  return result;
}

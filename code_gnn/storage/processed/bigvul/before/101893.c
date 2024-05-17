  const Extension* GetExtension() {
    const ExtensionList* extensions =
        browser()->profile()->GetExtensionService()->extensions();
    for (size_t i = 0; i < extensions->size(); ++i) {
      if ((*extensions)[i]->name() == "App Test")
        return (*extensions)[i];
    }
    NOTREACHED();
    return NULL;
  }

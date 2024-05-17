void AutomationProvider::GetEnabledExtensions(
    std::vector<FilePath>* result) {
  ExtensionService* service = profile_->GetExtensionService();
  DCHECK(service);
  if (service->extensions_enabled()) {
    const ExtensionList* extensions = service->extensions();
    DCHECK(extensions);
    for (size_t i = 0; i < extensions->size(); ++i) {
      const Extension* extension = (*extensions)[i];
      DCHECK(extension);
      if (!extension->is_app() &&
          (extension->location() == Extension::INTERNAL ||
           extension->location() == Extension::LOAD)) {
        result->push_back(extension->path());
      }
    }
  }
}

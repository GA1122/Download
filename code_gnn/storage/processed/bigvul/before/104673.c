  scoped_refptr<Extension> LoadExtensionWithLocation(
      const std::string& name,
      Extension::Location location,
      bool strict_error_checks,
      std::string* error) {
    scoped_ptr<DictionaryValue> value(LoadManifestFile(name, error));
    if (!value.get())
      return NULL;
    return LoadExtensionWithLocation(value.get(), location,
                                     strict_error_checks, error);
  }

  scoped_refptr<Extension> LoadExtensionStrict(const std::string& name,
                                               std::string* error) {
    return LoadExtensionWithLocation(name, Extension::INTERNAL, true, error);
  }

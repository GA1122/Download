scoped_refptr<Extension> Extension::Create(const FilePath& path,
                                           Location location,
                                           const DictionaryValue& value,
                                           int flags,
                                           std::string* error) {
  scoped_refptr<Extension> extension = new Extension(path, location);

  if (!extension->InitFromValue(value, flags, error))
    return NULL;
  return extension;
}

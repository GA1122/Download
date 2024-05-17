scoped_refptr<Extension> Extension::Create(const FilePath& path,
                                           Manifest::Location location,
                                           const DictionaryValue& value,
                                           int flags,
                                           std::string* utf8_error) {
  return Extension::Create(path,
                           location,
                           value,
                           flags,
                           std::string(),   
                           utf8_error);
}

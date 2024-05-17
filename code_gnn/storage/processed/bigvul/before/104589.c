scoped_refptr<Extension> Extension::CreateWithId(const FilePath& path,
                                                 Location location,
                                                 const DictionaryValue& value,
                                                 int flags,
                                                 const std::string& explicit_id,
                                                 std::string* error) {
  scoped_refptr<Extension> extension = Create(
      path, location, value, flags, error);
  if (extension.get())
    extension->id_ = explicit_id;
  return extension;
}

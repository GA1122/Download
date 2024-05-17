scoped_refptr<Extension> Extension::Create(const FilePath& path,
                                           Manifest::Location location,
                                           const DictionaryValue& value,
                                           int flags,
                                           const std::string& explicit_id,
                                           std::string* utf8_error) {
  DCHECK(utf8_error);
  string16 error;
  scoped_ptr<extensions::Manifest> manifest(
      new extensions::Manifest(location,
                               scoped_ptr<DictionaryValue>(value.DeepCopy())));

  if (!InitExtensionID(manifest.get(), path, explicit_id, flags, &error)) {
    *utf8_error = UTF16ToUTF8(error);
    return NULL;
  }

  std::vector<InstallWarning> install_warnings;
  manifest->ValidateManifest(utf8_error, &install_warnings);
  if (!utf8_error->empty())
    return NULL;

  scoped_refptr<Extension> extension = new Extension(path, manifest.Pass());
  extension->install_warnings_.swap(install_warnings);

  if (!extension->InitFromValue(flags, &error)) {
    *utf8_error = UTF16ToUTF8(error);
    return NULL;
  }

  if (!extension->CheckPlatformAppFeatures(utf8_error) ||
      !extension->CheckConflictingFeatures(utf8_error)) {
    return NULL;
  }

  return extension;
}

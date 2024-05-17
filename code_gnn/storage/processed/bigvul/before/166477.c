 scoped_refptr<const Extension> CreateExtensionWithPermissions(
    std::unique_ptr<base::Value> permissions,
    const std::string& name,
    bool allow_file_access) {
  int creation_flags = Extension::NO_FLAGS;
  if (allow_file_access)
    creation_flags |= Extension::ALLOW_FILE_ACCESS;
  return ExtensionBuilder()
      .SetLocation(Manifest::INTERNAL)
      .SetManifest(DictionaryBuilder()
                       .Set("name", name)
                       .Set("description", "foo")
                       .Set("manifest_version", 2)
                       .Set("version", "0.1.2.3")
                       .Set("permissions", std::move(permissions))
                       .Build())
      .AddFlags(creation_flags)
      .SetID(crx_file::id_util::GenerateId(name))
      .Build();
}

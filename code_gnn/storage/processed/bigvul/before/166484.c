scoped_refptr<const Extension> CreateExtensionWithOptionalPermissions(
    std::unique_ptr<base::Value> optional_permissions,
    std::unique_ptr<base::Value> permissions,
    const std::string& name) {
  return ExtensionBuilder()
      .SetLocation(Manifest::INTERNAL)
      .SetManifest(
          DictionaryBuilder()
              .Set("name", name)
              .Set("description", "foo")
              .Set("manifest_version", 2)
              .Set("version", "0.1.2.3")
              .Set("permissions", std::move(permissions))
              .Set("optional_permissions", std::move(optional_permissions))
              .Build())
      .SetID(crx_file::id_util::GenerateId(name))
      .Build();
}

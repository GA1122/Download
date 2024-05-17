scoped_refptr<const Extension> CreateTestExtension(
    const std::string& id,
    bool has_active_tab_permission,
    bool has_tab_capture_permission) {
  ListBuilder permissions;
  if (has_active_tab_permission)
    permissions.Append("activeTab");
  if (has_tab_capture_permission)
    permissions.Append("tabCapture");
  return ExtensionBuilder()
      .SetManifest(DictionaryBuilder()
                       .Set("name", "Extension with ID " + id)
                       .Set("version", "1.0")
                       .Set("manifest_version", 2)
                       .Set("permissions", permissions.Build())
                       .Build())
      .SetID(id)
      .Build();
}

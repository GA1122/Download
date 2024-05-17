scoped_refptr<const Extension> CreateTestExtension(
    const std::string& name,
    bool has_active_tab_permission,
    bool has_tab_capture_permission) {
  ExtensionBuilder builder(name);
  if (has_active_tab_permission)
    builder.AddPermission("activeTab");
  if (has_tab_capture_permission)
    builder.AddPermission("tabCapture");

  return builder.Build();
}

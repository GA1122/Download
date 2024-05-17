bool ExtensionRegistry::RemoveEnabled(const std::string& id) {
  return enabled_extensions_.Remove(id);
}

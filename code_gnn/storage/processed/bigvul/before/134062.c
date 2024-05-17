bool ExtensionRegistry::RemoveBlocked(const std::string& id) {
  return blocked_extensions_.Remove(id);
}

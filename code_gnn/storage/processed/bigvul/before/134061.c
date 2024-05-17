bool ExtensionRegistry::RemoveBlacklisted(const std::string& id) {
  return blacklisted_extensions_.Remove(id);
}

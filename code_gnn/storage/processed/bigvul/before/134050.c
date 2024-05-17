bool ExtensionRegistry::AddBlocked(
    const scoped_refptr<const Extension>& extension) {
  return blocked_extensions_.Insert(extension);
}

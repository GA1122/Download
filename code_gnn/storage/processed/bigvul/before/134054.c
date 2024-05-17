bool ExtensionRegistry::AddTerminated(
    const scoped_refptr<const Extension>& extension) {
  return terminated_extensions_.Insert(extension);
}

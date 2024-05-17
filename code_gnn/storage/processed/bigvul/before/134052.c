 bool ExtensionRegistry::AddEnabled(
     const scoped_refptr<const Extension>& extension) {
   return enabled_extensions_.Insert(extension);
}

bool PermissionsData::RequiresActionForScriptExecution(
    const Extension* extension) const {
  return RequiresActionForScriptExecution(extension, -1, GURL());
}

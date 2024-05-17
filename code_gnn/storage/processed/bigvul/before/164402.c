bool PermissionsData::CanExecuteScriptEverywhere(
    const ExtensionId& extension_id,
    Manifest::Location location) {
  if (location == Manifest::COMPONENT)
    return true;

  const ExtensionsClient::ScriptingWhitelist& whitelist =
      ExtensionsClient::Get()->GetScriptingWhitelist();

  return base::ContainsValue(whitelist, extension_id);
}

bool PermissionsData::RequiresActionForScriptExecution(
    const Extension* extension,
    int tab_id,
    const GURL& url) const {
  if (!extension->ShouldDisplayInExtensionSettings() ||
      Manifest::IsPolicyLocation(extension->location()) ||
      Manifest::IsComponentLocation(extension->location()) ||
      CanExecuteScriptEverywhere(extension) ||
      !active_permissions()->ShouldWarnAllHosts()) {
    return false;
  }

  if (HasTabSpecificPermissionToExecuteScript(tab_id, url))
    return false;

  return true;
}

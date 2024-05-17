bool Extension::IsComponentOnlyPermission(
    const ExtensionAPIPermission* api) const {
  if (location() == Extension::COMPONENT)
    return true;

  if (api == NULL)
    return true;

  return !api->is_component_only();
}

bool WebRequestPermissions::CanExtensionAccessInitiator(
    const extensions::InfoMap* extension_info_map,
    const extensions::ExtensionId extension_id,
    const base::Optional<url::Origin>& initiator,
    int tab_id,
    bool crosses_incognito) {
  PermissionsData::AccessType access = PermissionsData::ACCESS_ALLOWED;
  if (initiator) {
    access = CanExtensionAccessURL(
        extension_info_map, extension_id, initiator->GetURL(), tab_id,
        crosses_incognito, WebRequestPermissions::REQUIRE_HOST_PERMISSION,
        base::nullopt);
  }
  return access == PermissionsData::ACCESS_ALLOWED;
}

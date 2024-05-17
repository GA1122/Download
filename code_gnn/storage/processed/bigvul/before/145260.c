void Dispatcher::UpdateContentCapabilities(ScriptContext* context) {
  APIPermissionSet permissions;
  for (const auto& extension :
       *RendererExtensionRegistry::Get()->GetMainThreadExtensionSet()) {
    const ContentCapabilitiesInfo& info =
        ContentCapabilitiesInfo::Get(extension.get());
    if (info.url_patterns.MatchesURL(context->url())) {
      APIPermissionSet new_permissions;
      APIPermissionSet::Union(permissions, info.permissions, &new_permissions);
      permissions = new_permissions;
    }
  }
  context->set_content_capabilities(permissions);
}

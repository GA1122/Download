void Dispatcher::OnUpdatePermissions(
    const ExtensionMsg_UpdatePermissions_Params& params) {
  const Extension* extension =
      RendererExtensionRegistry::Get()->GetByID(params.extension_id);
  if (!extension)
    return;

  scoped_ptr<const PermissionSet> active =
      params.active_permissions.ToPermissionSet();
  scoped_ptr<const PermissionSet> withheld =
      params.withheld_permissions.ToPermissionSet();

  UpdateOriginPermissions(
      extension->url(),
      extension->permissions_data()->GetEffectiveHostPermissions(),
      active->effective_hosts());

  extension->permissions_data()->SetPermissions(std::move(active),
                                                std::move(withheld));
  UpdateBindings(extension->id());
}

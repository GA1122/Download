bool URLHasExtensionBackgroundPermission(
    extensions::ProcessMap* process_map,
    extensions::ExtensionRegistry* registry,
    const GURL& url,
    int opener_render_process_id) {
  const Extension* extension =
      registry->enabled_extensions().GetExtensionOrAppByURL(url);
  return extension &&
         extension->permissions_data()->HasAPIPermission(
             APIPermission::kBackground) &&
         process_map->Contains(extension->id(), opener_render_process_id);
}

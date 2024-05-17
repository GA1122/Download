ExtensionAppItem::ExtensionAppItem(
    Profile* profile,
    const app_list::AppListSyncableService::SyncItem* sync_item,
    const std::string& extension_id,
    const std::string& extension_name,
    const gfx::ImageSkia& installing_icon,
    bool is_platform_app)
    : app_list::AppListItem(extension_id),
      profile_(profile),
      extension_id_(extension_id),
      extension_enable_flow_controller_(NULL),
      extension_name_(extension_name),
      installing_icon_(CreateDisabledIcon(installing_icon)),
      is_platform_app_(is_platform_app),
      has_overlay_(false) {
  Reload();
  if (sync_item && sync_item->item_ordinal.IsValid()) {
    set_position(sync_item->item_ordinal);
    if (name().empty())
      SetName(sync_item->item_name);
    return;
  }
  GetAppSorting(profile_)->EnsureValidOrdinals(extension_id_,
                                               syncer::StringOrdinal());
  UpdatePositionFromExtensionOrdering();
}

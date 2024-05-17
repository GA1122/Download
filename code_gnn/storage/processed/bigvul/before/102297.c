void ExtensionPrefs::OnExtensionMoved(
    const std::string& moved_extension_id,
    const std::string& predecessor_extension_id,
    const std::string& successor_extension_id) {
  if (predecessor_extension_id.empty() && successor_extension_id.empty())
    return;

  if (predecessor_extension_id.empty()) {
    SetAppLaunchOrdinal(
        moved_extension_id,
        GetAppLaunchOrdinal(successor_extension_id).CreateBefore());
  } else if (successor_extension_id.empty()) {
    SetAppLaunchOrdinal(
        moved_extension_id,
        GetAppLaunchOrdinal(predecessor_extension_id).CreateAfter());
  } else {
    const StringOrdinal& predecessor_ordinal =
        GetAppLaunchOrdinal(predecessor_extension_id);
    const StringOrdinal& successor_ordinal =
        GetAppLaunchOrdinal(successor_extension_id);
    SetAppLaunchOrdinal(moved_extension_id,
                        predecessor_ordinal.CreateBetween(successor_ordinal));
  }

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_LAUNCHER_REORDERED,
      content::Source<ExtensionPrefs>(this),
      content::NotificationService::NoDetails());
}

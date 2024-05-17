void ExtensionAppItem::UpdatePositionFromExtensionOrdering() {
  const syncer::StringOrdinal& page =
      GetAppSorting(profile_)->GetPageOrdinal(extension_id_);
  const syncer::StringOrdinal& launch =
     GetAppSorting(profile_)->GetAppLaunchOrdinal(extension_id_);
  set_position(syncer::StringOrdinal(
      page.ToInternalValue() + launch.ToInternalValue()));
}

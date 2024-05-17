base::string16 GetDisplayRemovedMessage(
    const display::ManagedDisplayInfo& removed_display_info,
    base::string16* out_additional_message) {
  return l10n_util::GetStringFUTF16(
      IDS_ASH_STATUS_TRAY_DISPLAY_REMOVED,
      base::UTF8ToUTF16(removed_display_info.name()));
}

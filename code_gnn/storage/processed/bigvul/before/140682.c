base::string16 GetExternalDisplayName(int64_t external_display_id) {
  DCHECK(!display::Display::IsInternalDisplayId(external_display_id));

  display::DisplayManager* display_manager = GetDisplayManager();
  DCHECK(!display_manager->IsInMirrorMode());

  if (external_display_id == display::kInvalidDisplayId)
    return l10n_util::GetStringUTF16(IDS_DISPLAY_NAME_UNKNOWN);

  base::string16 name = GetDisplayName(external_display_id);
  const display::ManagedDisplayInfo& display_info =
      display_manager->GetDisplayInfo(external_display_id);
  if (display_info.GetActiveRotation() != display::Display::ROTATE_0 ||
      display_info.configured_ui_scale() != 1.0f ||
      !display_info.overscan_insets_in_dip().IsEmpty()) {
    name =
        l10n_util::GetStringFUTF16(IDS_ASH_STATUS_TRAY_DISPLAY_ANNOTATED_NAME,
                                   name, GetDisplaySize(external_display_id));
  } else if (display_info.overscan_insets_in_dip().IsEmpty() &&
             display_info.has_overscan()) {
    name = l10n_util::GetStringFUTF16(
        IDS_ASH_STATUS_TRAY_DISPLAY_ANNOTATED_NAME, name,
        l10n_util::GetStringUTF16(
            IDS_ASH_STATUS_TRAY_DISPLAY_ANNOTATION_OVERSCAN));
  }

  return name;
}

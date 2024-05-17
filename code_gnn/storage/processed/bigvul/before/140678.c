base::string16 GetEnterMirrorModeMessage() {
  if (display::Display::HasInternalDisplay()) {
    return l10n_util::GetStringFUTF16(
        IDS_ASH_STATUS_TRAY_DISPLAY_MIRRORING,
        GetDisplayName(GetDisplayManager()->mirroring_display_id()));
  }

  return l10n_util::GetStringUTF16(
      IDS_ASH_STATUS_TRAY_DISPLAY_MIRRORING_NO_INTERNAL);
}

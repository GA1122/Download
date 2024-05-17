bool ScreenLayoutObserver::GetExitMirrorModeMessage(
    base::string16* out_message,
    base::string16* out_additional_message) {
  switch (current_display_mode_) {
    case DisplayMode::EXTENDED_3_PLUS:
      *out_message =
          l10n_util::GetStringUTF16(IDS_ASH_DISPLAY_MIRRORING_NOT_SUPPORTED);
      return true;

    case DisplayMode::DOCKED:
    case DisplayMode::SINGLE:
      *out_message =
          l10n_util::GetStringUTF16(IDS_ASH_STATUS_TRAY_DISPLAY_MIRROR_EXIT);
      return true;

    default:
      return false;
  }
}

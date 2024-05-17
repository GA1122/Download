base::string16 GetDisplayAddedMessage(int64_t added_display_id,
                                      base::string16* additional_message_out) {
  if (!display::Display::HasInternalDisplay()) {
    return l10n_util::GetStringUTF16(
        IDS_ASH_STATUS_TRAY_DISPLAY_EXTENDED_NO_INTERNAL);
  }

  return l10n_util::GetStringFUTF16(IDS_ASH_STATUS_TRAY_DISPLAY_EXTENDED,
                                    GetExternalDisplayName(added_display_id));
}

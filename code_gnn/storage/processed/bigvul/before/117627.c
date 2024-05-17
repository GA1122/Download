string16 AutolaunchInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return l10n_util::GetStringUTF16((button == BUTTON_OK) ?
      IDS_AUTO_LAUNCH_OK : IDS_AUTO_LAUNCH_REVERT);
}

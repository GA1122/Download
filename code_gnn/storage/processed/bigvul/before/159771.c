base::string16 PermissionsBubbleDialogDelegateView::GetDialogButtonLabel(
    ui::DialogButton button) const {
  if (button == ui::DIALOG_BUTTON_CANCEL)
    return l10n_util::GetStringUTF16(IDS_PERMISSION_DENY);

  return l10n_util::GetStringUTF16(GetDialogButtons() == ui::DIALOG_BUTTON_OK
                                       ? IDS_OK
                                       : IDS_PERMISSION_ALLOW);
}

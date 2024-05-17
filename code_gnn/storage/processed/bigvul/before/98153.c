string16 AutoFillCCInfoBarDelegate::GetButtonLabel(
    ConfirmInfoBarDelegate::InfoBarButton button) const {
  if (button == BUTTON_OK)
    return l10n_util::GetStringUTF16(IDS_AUTOFILL_CC_INFOBAR_ACCEPT);
  else if (button == BUTTON_CANCEL)
    return l10n_util::GetStringUTF16(IDS_AUTOFILL_CC_INFOBAR_DENY);
  else
    NOTREACHED();

  return string16();
}

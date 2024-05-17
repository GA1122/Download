InfoBar* CreateAutofillCcInfoBar(ConfirmInfoBarDelegate* delegate) {
  DCHECK(delegate);
  return new SaveCCInfoConfirmInfoBar(delegate);
}

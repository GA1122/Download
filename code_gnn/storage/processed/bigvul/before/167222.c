void ExtensionDevToolsInfoBarDelegate::InfoBarDismissed() {
  DCHECK(!dismissed_callback_.is_null());
  base::ResetAndReturn(&dismissed_callback_).Run();
}

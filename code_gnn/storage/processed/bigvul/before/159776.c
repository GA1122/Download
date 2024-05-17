base::string16 PermissionsBubbleDialogDelegateView::GetWindowTitle() const {
  return l10n_util::GetStringFUTF16(IDS_PERMISSIONS_BUBBLE_PROMPT,
                                    display_origin_);
}

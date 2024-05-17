base::string16 PermissionsBubbleDialogDelegateView::GetAccessibleWindowTitle()
    const {
  return l10n_util::GetStringFUTF16(IDS_PERMISSIONS_BUBBLE_ACCESSIBLE_TITLE,
                                    display_origin_);
}

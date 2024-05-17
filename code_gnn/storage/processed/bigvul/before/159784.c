void PermissionsBubbleDialogDelegateView::UpdateAnchor() {
  views::View* anchor_view = GetPermissionAnchorView(owner_->browser());
  SetAnchorView(anchor_view);
  if (!anchor_view)
    SetAnchorRect(GetPermissionAnchorRect(owner_->browser()));
}

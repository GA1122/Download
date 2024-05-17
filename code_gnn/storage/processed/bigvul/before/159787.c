PermissionsBubbleDialogDelegateView::~PermissionsBubbleDialogDelegateView() {
  if (owner_)
    owner_->Closing();
}

void PermissionsBubbleDialogDelegateView::CloseBubble() {
  owner_ = nullptr;
   GetWidget()->Close();
 }

void PermissionPromptImpl::Show() {
  DCHECK(browser_);
  DCHECK(browser_->window());

  bubble_delegate_ =
      new PermissionsBubbleDialogDelegateView(this, delegate_->Requests());

  bubble_delegate_->set_parent_window(
      platform_util::GetViewForWindow(browser_->window()->GetNativeWindow()));

  bubble_delegate_->set_anchor_view_insets(gfx::Insets(
      GetLayoutConstant(LOCATION_BAR_BUBBLE_ANCHOR_VERTICAL_INSET), 0));

  views::Widget* widget =
      views::BubbleDialogDelegateView::CreateBubble(bubble_delegate_);
  if (browser_->window()->IsActive())
    widget->Show();
  else
    widget->ShowInactive();

  bubble_delegate_->SizeToContents();

  bubble_delegate_->UpdateAnchor();
}

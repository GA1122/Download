void AutofillDialogViews::ContentsPreferredSizeChanged() {
  if (updates_scope_ != 0) {
    needs_update_ = true;
    return;
  }

  preferred_size_ = gfx::Size();

  if (GetWidget() && delegate_ && delegate_->GetWebContents()) {
    constrained_window::UpdateWebContentsModalDialogPosition(
        GetWidget(),
        web_modal::WebContentsModalDialogManager::FromWebContents(
            delegate_->GetWebContents())->delegate()->
                GetWebContentsModalDialogHost());
    SetBoundsRect(bounds());
  }
}

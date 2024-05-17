void BrowserActionsContainer::BubbleGotFocus(BrowserBubble* bubble) {
  if (!popup_)
    return;

  popup_->host()->render_view_host()->view()->Focus();
}

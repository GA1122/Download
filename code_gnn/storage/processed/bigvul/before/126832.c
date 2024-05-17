void BrowserView::OnSysColorChange() {
  chrome::MaybeShowInvertBubbleView(browser_.get(), contents_);
}

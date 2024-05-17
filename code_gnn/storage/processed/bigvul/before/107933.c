void InfoBar::Close() {
  GetParent()->RemoveChildView(this);
  if (delegate_) {
    delegate_->InfoBarClosed();
    delegate_ = NULL;
  }
}

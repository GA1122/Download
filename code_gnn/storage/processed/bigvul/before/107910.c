void ExtensionInfoBar::OnDelegateDeleted() {
  delegate_->extension_host()->view()->SetContainer(NULL);
  delegate_ = NULL;
}

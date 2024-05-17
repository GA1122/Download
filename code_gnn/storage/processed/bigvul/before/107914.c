ExtensionInfoBar::~ExtensionInfoBar() {
  if (delegate_) {
    delegate_->extension_host()->view()->SetContainer(NULL);
    delegate_->set_observer(NULL);
  }
}

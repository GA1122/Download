void AXObjectCacheImpl::listboxOptionStateChanged(HTMLOptionElement* option) {
  postNotification(option, AXCheckedStateChanged);
}

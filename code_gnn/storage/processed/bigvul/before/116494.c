void ExtensionDevToolsClientHost::InfoBarDestroyed() {
  infobar_delegate_ = NULL;
  SendDetachedEvent();
  Close();
}

TabContentsWrapper* DefaultTabHandler::AddBlankTab(bool foreground) {
  UmaNaclHistogramEnumeration(NEW_TAB_NACL_BASELINE);
  return delegate_->AsBrowser()->AddBlankTab(foreground);
}

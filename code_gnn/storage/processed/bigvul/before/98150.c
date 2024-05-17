bool AutoFillCCInfoBarDelegate::Accept() {
  UMA_HISTOGRAM_COUNTS("AutoFill.CCInfoBarAccepted", 1);
  if (host_) {
    host_->OnInfoBarClosed(true);
    host_ = NULL;
  }
  return true;
}

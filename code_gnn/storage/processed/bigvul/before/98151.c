bool AutoFillCCInfoBarDelegate::Cancel() {
  UMA_HISTOGRAM_COUNTS("AutoFill.CCInfoBarDenied", 1);
  if (host_) {
    host_->OnInfoBarClosed(false);
    host_ = NULL;
  }
  return true;
}

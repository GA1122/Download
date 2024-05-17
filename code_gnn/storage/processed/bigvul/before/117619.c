bool DefaultBrowserInfoBarDelegate::Cancel() {
  action_taken_ = true;
  UMA_HISTOGRAM_COUNTS("DefaultBrowserWarning.DontSetAsDefault", 1);
  prefs_->SetBoolean(prefs::kCheckDefaultBrowser, false);
  return true;
}

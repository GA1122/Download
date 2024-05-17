bool DefaultBrowserInfoBarDelegate::Accept() {
  action_taken_ = true;
  UMA_HISTOGRAM_COUNTS("DefaultBrowserWarning.SetAsDefault", 1);
  BrowserThread::PostTask(
      BrowserThread::FILE,
      FROM_HERE,
      base::Bind(base::IgnoreResult(&ShellIntegration::SetAsDefaultBrowser)));
  return true;
}

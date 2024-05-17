InfoBarService* DefaultBindingsDelegate::GetInfoBarService() {
  return InfoBarService::FromWebContents(web_contents_);
}

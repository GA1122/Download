void BrowserView::OnLoadCompleted() {
#if defined(OS_WIN)
  JumpListFactory::GetForProfile(browser_->profile());
#endif
}

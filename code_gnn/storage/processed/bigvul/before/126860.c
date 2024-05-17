void BrowserView::ShowBackgroundPages() {
#if defined(USE_AURA)
  TaskManagerDialog::ShowBackgroundPages();
#else
  chrome::ShowBackgroundPages();
#endif   
}

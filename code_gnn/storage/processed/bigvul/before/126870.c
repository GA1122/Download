void BrowserView::ShowTaskManager() {
#if defined(USE_AURA)
  TaskManagerDialog::Show();
#else
  chrome::ShowTaskManager();
#endif   
}

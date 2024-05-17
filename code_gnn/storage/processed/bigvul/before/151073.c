content::JavaScriptDialogManager* DevToolsWindow::GetJavaScriptDialogManager(
    WebContents* source) {
  return app_modal::JavaScriptDialogManager::GetInstance();
}

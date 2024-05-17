ChromeWebContentsDelegateAndroid::GetJavaScriptDialogManager(
    WebContents* source) {
  return app_modal::JavaScriptDialogManager::GetInstance();
}

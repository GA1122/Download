content::JavaScriptDialogManager* Browser::GetJavaScriptDialogManager(
    WebContents* source) {
  return JavaScriptDialogTabHelper::FromWebContents(source);
}

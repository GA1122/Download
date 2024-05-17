bool BrowserView::DoCutCopyPaste(void (content::RenderWidgetHost::*method)()) {
#if defined(USE_AURA)
  WebContents* contents = chrome::GetActiveWebContents(browser_.get());
  if (contents && contents->GetContentNativeView() &&
      contents->GetContentNativeView()->HasFocus()) {
    (contents->GetRenderViewHost()->*method)();
    return true;
  }
#elif defined(OS_WIN)
#endif
  return false;
}

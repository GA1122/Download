void WebLocalFrameImpl::ReplaceMisspelledRange(const WebString& text) {
  if (GetFrame()->GetWebPluginContainer())
    return;

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  GetFrame()->GetSpellChecker().ReplaceMisspelledRange(text);
}

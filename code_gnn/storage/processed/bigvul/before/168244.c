bool BrowserView::DoCutCopyPasteForWebContents(
    WebContents* contents,
    void (WebContents::*method)()) {
  content::RenderWidgetHostView* rwhv = contents->GetRenderWidgetHostView();
  if (!rwhv || !rwhv->HasFocus())
    return false;
  (contents->*method)();
  return true;
}

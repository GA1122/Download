void BrowserView::OnWindowBeginUserBoundsChange() {
  WebContents* web_contents = GetActiveWebContents();
  if (!web_contents)
    return;
  web_contents->GetRenderViewHost()->NotifyMoveOrResizeStarted();
}

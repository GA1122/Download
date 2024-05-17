void BrowserView::OnWindowBeginUserBoundsChange() {
  if (interactive_resize_)
    return;
  WebContents* web_contents = GetActiveWebContents();
  if (!web_contents)
    return;
  interactive_resize_ = ResizeSession();
  interactive_resize_->begin_timestamp = base::TimeTicks::Now();
  web_contents->GetRenderViewHost()->NotifyMoveOrResizeStarted();
}

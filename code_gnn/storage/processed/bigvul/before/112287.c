bool ShellWindow::IsFullscreenForTabOrPending(
    const content::WebContents* source) const {
  DCHECK(source == web_contents_);
  return IsFullscreenOrPending();
}

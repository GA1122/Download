void ShellWindow::NavigationStateChanged(
    const content::WebContents* source, unsigned changed_flags) {
  DCHECK(source == web_contents_);
  if (changed_flags & content::INVALIDATE_TYPE_TITLE)
    UpdateWindowTitle();
}

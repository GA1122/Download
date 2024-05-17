bool UnloadController::CanCloseContents(content::WebContents* contents) {
  if (is_attempting_to_close_browser_)
    ClearUnloadState(contents, true);
  return !is_attempting_to_close_browser_;
}

bool DevToolsWindow::InterceptPageBeforeUnload(WebContents* contents) {
  DevToolsWindow* window =
      DevToolsWindow::GetInstanceForInspectedWebContents(contents);
  if (!window || window->intercepted_page_beforeunload_)
    return false;

  if (window->life_stage_ != kLoadCompleted)
    return false;

  window->intercepted_page_beforeunload_ = true;
  if (!DevToolsWindow::InterceptPageBeforeUnload(window->main_web_contents_)) {
    window->main_web_contents_->DispatchBeforeUnload();
  }
  return true;
}

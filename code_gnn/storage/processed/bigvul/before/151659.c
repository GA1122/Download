void Browser::CloseContents(WebContents* source) {
  bool can_close_contents;
  if (IsFastTabUnloadEnabled())
    can_close_contents = fast_unload_controller_->CanCloseContents(source);
  else
    can_close_contents = unload_controller_->CanCloseContents(source);

  if (can_close_contents)
    chrome::CloseWebContents(this, source, true);
}

void Browser::CloseContents(WebContents* source) {
  if (unload_controller_->CanCloseContents(source))
    chrome::CloseWebContents(this, source);
}

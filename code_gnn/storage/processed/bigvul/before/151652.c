void Browser::BeforeUnloadFired(WebContents* web_contents,
                                bool proceed,
                                bool* proceed_to_fire_unload) {
  if (is_devtools() && DevToolsWindow::HandleBeforeUnload(web_contents,
        proceed, proceed_to_fire_unload))
    return;

  if (IsFastTabUnloadEnabled()) {
    *proceed_to_fire_unload =
        fast_unload_controller_->BeforeUnloadFired(web_contents, proceed);
  } else {
    *proceed_to_fire_unload =
        unload_controller_->BeforeUnloadFired(web_contents, proceed);
  }
 }

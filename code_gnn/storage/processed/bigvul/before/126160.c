void Browser::BeforeUnloadFired(WebContents* web_contents,
                                bool proceed,
                                bool* proceed_to_fire_unload) {
  *proceed_to_fire_unload =
      unload_controller_->BeforeUnloadFired(web_contents, proceed);
}

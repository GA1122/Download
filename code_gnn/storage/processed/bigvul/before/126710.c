UnloadController::UnloadController(Browser* browser)
    : browser_(browser),
      is_attempting_to_close_browser_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)) {
  browser_->tab_strip_model()->AddObserver(this);
}

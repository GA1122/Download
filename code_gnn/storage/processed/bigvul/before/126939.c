BrowserTabStripController::~BrowserTabStripController() {
  if (context_menu_contents_.get())
    context_menu_contents_->Cancel();

  model_->RemoveObserver(this);
}

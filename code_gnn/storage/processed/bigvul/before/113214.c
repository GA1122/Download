LauncherView::~LauncherView() {
  bounds_animator_->RemoveObserver(this);
  model_->RemoveObserver(this);
}

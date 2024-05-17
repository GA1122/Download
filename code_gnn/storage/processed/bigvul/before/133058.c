  virtual ~EventFilterForPopupExit() {
    aura::Window* root_window = rwhva_->window_->GetRootWindow();
    DCHECK(root_window);
    root_window->RemovePreTargetHandler(this);
  }

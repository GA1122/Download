  virtual ~MinimalInputEventFilter() {
    root_->window()->RemovePreTargetHandler(this);
    root_->window()->SetProperty(aura::client::kRootWindowInputMethodKey,
                       static_cast<ui::InputMethod*>(NULL));
  }

  explicit MinimalInputEventFilter(aura::RootWindow* root)
      : root_(root),
        input_method_(ui::CreateInputMethod(this,
                                            gfx::kNullAcceleratedWidget)) {
    input_method_->Init(true);
    root_->window()->AddPreTargetHandler(this);
    root_->window()->SetProperty(aura::client::kRootWindowInputMethodKey,
                       input_method_.get());
  }

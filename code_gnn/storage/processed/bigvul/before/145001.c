  explicit WindowAncestorObserver(RenderWidgetHostViewAura* view)
      : view_(view) {
    aura::Window* parent = view_->window_->parent();
    while (parent) {
      parent->AddObserver(this);
      ancestors_.insert(parent);
      parent = parent->parent();
    }
  }

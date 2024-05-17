  void UpdateConstrainedWindows(aura::Window* exclude) {
    RenderWidgetHostViewAura* view = ToRenderWidgetHostViewAura(
        view_->web_contents_->GetRenderWidgetHostView());
    if (!view)
      return;

    std::vector<gfx::Rect> constrained_windows;
    if (parent_) {
      const aura::Window::Windows& children = parent_->children();
      for (size_t i = 0; i < children.size(); ++i) {
        if (children[i] != view_->window_ &&
            children[i] != exclude &&
            children[i]->IsVisible()) {
          constrained_windows.push_back(children[i]->GetBoundsInRootWindow());
        }
      }
    }

    aura::Window* root_window = view_->window_->GetRootWindow();
    const aura::Window::Windows& root_children = root_window->children();
    if (root_window) {
      for (size_t i = 0; i < root_children.size(); ++i) {
        if (root_children[i]->IsVisible() &&
            !root_children[i]->Contains(view_->window_.get())) {
          constrained_windows.push_back(
              root_children[i]->GetBoundsInRootWindow());
        }
      }
    }

    view->UpdateConstrainedWindowRects(constrained_windows);
  }

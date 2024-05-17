  virtual ~WindowObserver() {
    view_->window_->RemoveObserver(this);
    if (view_->window_->GetDispatcher())
      view_->window_->GetDispatcher()->RemoveRootWindowObserver(this);
    if (parent_)
      parent_->RemoveObserver(this);

#if defined(OS_WIN)
    if (parent_) {
      const aura::Window::Windows& children = parent_->children();
      for (size_t i = 0; i < children.size(); ++i)
        children[i]->RemoveObserver(this);
    }

    aura::Window* root_window = view_->window_->GetRootWindow();
    if (root_window) {
      root_window->RemoveObserver(this);
      const aura::Window::Windows& root_children = root_window->children();
      for (size_t i = 0; i < root_children.size(); ++i)
        root_children[i]->RemoveObserver(this);
    }
#endif
  }

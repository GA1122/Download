  explicit WindowObserver(WebContentsViewAura* view)
      : view_(view),
        parent_(NULL) {
    view_->window_->AddObserver(this);

#if defined(OS_WIN)
    if (view_->window_->GetRootWindow())
      view_->window_->GetRootWindow()->AddObserver(this);
#endif
  }

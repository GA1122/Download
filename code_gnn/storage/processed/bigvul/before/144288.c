  explicit CloseAfterCommit(views::Widget* widget) : widget_(widget) {
    widget->GetCompositor()->AddObserver(this);
    widget_->AddObserver(this);
  }

  bool showing_filter_widget() {
    return window_selector()
        ->text_filter_widget_->GetNativeWindow()
        ->layer()
        ->GetTargetTransform()
        .IsIdentity();
  }

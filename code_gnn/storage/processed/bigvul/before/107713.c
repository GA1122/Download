void StatusBubbleGtk::UpdateLabelSizeRequest() {
  if (!expanded() || !expand_animation_->is_animating()) {
    gtk_widget_set_size_request(label_, -1, -1);
    return;
  }

  int new_width = start_width_ +
      (desired_width_ - start_width_) * expand_animation_->GetCurrentValue();
  gtk_widget_set_size_request(label_, new_width, -1);
}

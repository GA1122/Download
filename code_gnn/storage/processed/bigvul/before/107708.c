 void StatusBubbleGtk::SetStatusTextTo(const std::string& status_utf8) {
  if (status_utf8.empty()) {
    hide_timer_.Stop();
    hide_timer_.Start(base::TimeDelta::FromMilliseconds(kHideDelay),
                      this, &StatusBubbleGtk::Hide);
  } else {
    gtk_label_set_text(GTK_LABEL(label_), status_utf8.c_str());
    GtkRequisition req;
    gtk_widget_size_request(label_, &req);
    desired_width_ = req.width;

    UpdateLabelSizeRequest();

    if (!last_mouse_left_content_) {
      gtk_widget_show_all(padding_);
      MouseMoved(last_mouse_location_, false);
    }
    Show();
  }
}

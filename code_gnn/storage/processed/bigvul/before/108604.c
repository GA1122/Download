 void Shell::SizeTo(int width, int height) {
  content_width_ = width;
  content_height_ = height;
  if (web_contents_.get()) {
    gtk_widget_set_size_request(web_contents_->GetNativeView(), width, height);
  }
}

void TabContentsContainerGtk::SetPreview(TabContents* preview) {
  if (preview_ == preview)
    return;

  if (preview_) {
    HideTab(preview_);
    GtkWidget* preview_widget = preview_->web_contents()->GetNativeView();
    if (preview_widget)
      gtk_container_remove(GTK_CONTAINER(expanded_), preview_widget);
  }

  preview_ = preview;

  if (preview_)
    PackTab(preview_);
}

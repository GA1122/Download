void BrowserWindowGtk::QueueToolbarRedraw() {
  gtk_widget_queue_draw(toolbar_->widget());
}

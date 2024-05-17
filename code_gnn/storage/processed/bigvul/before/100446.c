void RenderThread::WidgetRestored() {
  DCHECK(hidden_widget_count_ > 0);
  hidden_widget_count_--;

}

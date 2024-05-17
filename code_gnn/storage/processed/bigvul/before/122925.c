void RenderProcessHostImpl::WidgetRestored() {
  DCHECK_EQ(backgrounded_, (visible_widgets_ == 0));
  visible_widgets_++;
  SetBackgrounded(false);
}

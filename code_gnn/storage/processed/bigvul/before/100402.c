void BrowserRenderProcessHost::WidgetRestored() {
  DCHECK_EQ(backgrounded_, (visible_widgets_ == 0));
  visible_widgets_++;
  visited_link_updater_->Update(this);
  SetBackgrounded(false);
}

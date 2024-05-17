void Instance::DestroyVerticalScrollbar() {
  if (!v_scrollbar_.get())
    return;
  if (v_scrollbar_->GetValue())
    engine_->ScrolledToYPosition(0);
  v_scrollbar_.reset();
  page_indicator_.Show(false, true);
}

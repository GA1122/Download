void RenderWidgetHostImpl::OnMsgSelectRangeAck() {
  select_range_pending_ = false;
  if (next_selection_range_.get()) {
    scoped_ptr<SelectionRange> next(next_selection_range_.Pass());
    SelectRange(next->start, next->end);
  }
}

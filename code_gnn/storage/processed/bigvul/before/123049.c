void RenderWidgetHostImpl::SelectRange(const gfx::Point& start,
                                       const gfx::Point& end) {
  if (select_range_pending_) {
    if (!next_selection_range_.get()) {
      next_selection_range_.reset(new SelectionRange());
    }
    next_selection_range_->start = start;
    next_selection_range_->end = end;
    return;
  }

  select_range_pending_ = true;
  Send(new ViewMsg_SelectRange(GetRoutingID(), start, end));
}

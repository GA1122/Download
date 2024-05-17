void BrowserView::InitStatusBubble() {
  status_bubble_.reset(
      new StatusBubbleViews(contents_web_view_, HasClientEdge()));
  contents_web_view_->SetStatusBubble(status_bubble_.get());
}

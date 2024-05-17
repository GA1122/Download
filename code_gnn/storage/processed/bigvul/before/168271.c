gfx::Size BrowserView::GetContentsSize() const {
  DCHECK(initialized_);
  return contents_web_view_->size();
}

void RenderViewHostImpl::OnSelectionChanged(const string16& text,
                                            size_t offset,
                                            const ui::Range& range) {
  if (view_)
    view_->SelectionChanged(text, offset, range);
}

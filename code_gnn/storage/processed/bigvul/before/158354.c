void RenderWidgetHostImpl::SelectionChanged(const base::string16& text,
                                            uint32_t offset,
                                            const gfx::Range& range) {
  if (view_)
    view_->SelectionChanged(text, static_cast<size_t>(offset), range);
}

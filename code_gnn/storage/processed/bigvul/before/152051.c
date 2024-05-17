void RenderFrameHostImpl::OnTextSurroundingSelectionResponse(
    const base::string16& content,
    uint32_t start_offset,
    uint32_t end_offset) {
  if (text_surrounding_selection_callback_.is_null())
    return;

  text_surrounding_selection_callback_.Run(content, start_offset, end_offset);
  text_surrounding_selection_callback_.Reset();
}

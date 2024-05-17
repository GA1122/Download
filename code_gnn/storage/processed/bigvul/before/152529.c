void RenderFrameImpl::SetSelectedText(const base::string16& selection_text,
                                      size_t offset,
                                      const gfx::Range& range) {
  Send(new FrameHostMsg_SelectionChanged(routing_id_, selection_text,
                                         static_cast<uint32_t>(offset), range));
}

void RenderFrameImpl::SetSelectedText(const base::string16& selection_text,
                                      size_t offset,
                                      const gfx::Range& range) {
  Send(new ViewHostMsg_SelectionChanged(GetRenderWidget()->routing_id(),
                                        selection_text,
                                        offset,
                                        range));
}

void RenderFrameHostImpl::RequestTextSurroundingSelection(
    const TextSurroundingSelectionCallback& callback,
    int max_length) {
  DCHECK(!callback.is_null());
  if (!text_surrounding_selection_callback_.is_null()) {
    callback.Run(base::string16(), 0, 0);
    return;
  }
  text_surrounding_selection_callback_ = callback;
  Send(
      new FrameMsg_TextSurroundingSelectionRequest(GetRoutingID(), max_length));
}

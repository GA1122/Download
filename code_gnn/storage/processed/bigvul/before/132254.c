void RenderFrameImpl::OnTextSurroundingSelectionRequest(size_t max_length) {
  blink::WebSurroundingText surroundingText;
  surroundingText.initialize(frame_->selectionRange(), max_length);

  if (surroundingText.isNull()) {
    Send(new FrameHostMsg_TextSurroundingSelectionResponse(
        routing_id_, base::string16(), 0, 0));
    return;
  }

  Send(new FrameHostMsg_TextSurroundingSelectionResponse(
      routing_id_,
      surroundingText.textContent(),
      surroundingText.startOffsetInTextContent(),
      surroundingText.endOffsetInTextContent()));
}

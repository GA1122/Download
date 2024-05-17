void RenderFrameImpl::OnTextSurroundingSelectionRequest(uint32_t max_length) {
  blink::WebSurroundingText surrounding_text(frame_, max_length);

  if (surrounding_text.IsEmpty()) {
    Send(new FrameHostMsg_TextSurroundingSelectionResponse(
        routing_id_, base::string16(), 0, 0));
    return;
  }

  Send(new FrameHostMsg_TextSurroundingSelectionResponse(
      routing_id_, surrounding_text.TextContent().Utf16(),
      surrounding_text.StartOffsetInTextContent(),
      surrounding_text.EndOffsetInTextContent()));
}

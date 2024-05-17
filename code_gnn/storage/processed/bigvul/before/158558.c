void WebLocalFrameImpl::MoveRangeSelection(
    const WebPoint& base_in_viewport,
    const WebPoint& extent_in_viewport,
    WebFrame::TextGranularity granularity) {
  TRACE_EVENT0("blink", "WebLocalFrameImpl::moveRangeSelection");

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  blink::TextGranularity blink_granularity = blink::TextGranularity::kCharacter;
  if (granularity == WebFrame::kWordGranularity)
    blink_granularity = blink::TextGranularity::kWord;
  GetFrame()->Selection().MoveRangeSelection(
      GetFrame()->View()->ViewportToFrame(base_in_viewport),
      GetFrame()->View()->ViewportToFrame(extent_in_viewport),
      blink_granularity);
}

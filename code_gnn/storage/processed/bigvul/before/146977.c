void WebLocalFrameImpl::ExtractSmartClipData(WebRect rect_in_viewport,
                                             WebString& clip_text,
                                             WebString& clip_html) {
  SmartClipData clip_data = SmartClip(GetFrame()).DataForRect(rect_in_viewport);
  clip_text = clip_data.ClipData();

  WebPoint start_point(rect_in_viewport.x, rect_in_viewport.y);
  WebPoint end_point(rect_in_viewport.x + rect_in_viewport.width,
                     rect_in_viewport.y + rect_in_viewport.height);
  VisiblePosition start_visible_position =
      VisiblePositionForViewportPoint(start_point);
  VisiblePosition end_visible_position =
      VisiblePositionForViewportPoint(end_point);

  Position start_position = start_visible_position.DeepEquivalent();
  Position end_position = end_visible_position.DeepEquivalent();

  if (!start_position.GetDocument() || !end_position.GetDocument())
    return;

  if (start_position.CompareTo(end_position) <= 0) {
    clip_html =
        CreateMarkup(start_position, end_position, kAnnotateForInterchange,
                     ConvertBlocksToInlines::kNotConvert, kResolveNonLocalURLs);
  } else {
    clip_html =
        CreateMarkup(end_position, start_position, kAnnotateForInterchange,
                     ConvertBlocksToInlines::kNotConvert, kResolveNonLocalURLs);
  }
}

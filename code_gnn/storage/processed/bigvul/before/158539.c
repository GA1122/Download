void WebLocalFrameImpl::ExtractSmartClipData(WebRect rect_in_viewport,
                                             WebString& clip_text,
                                             WebString& clip_html,
                                             WebRect& clip_rect) {
  SmartClipData clip_data = SmartClip(GetFrame()).DataForRect(rect_in_viewport);
  clip_text = clip_data.ClipData();
  clip_rect = clip_data.RectInViewport();

  WebPoint start_point(rect_in_viewport.x, rect_in_viewport.y);
  WebPoint end_point(rect_in_viewport.x + rect_in_viewport.width,
                     rect_in_viewport.y + rect_in_viewport.height);
  clip_html = CreateMarkupInRect(
      GetFrame(), GetFrame()->View()->ViewportToFrame(start_point),
      GetFrame()->View()->ViewportToFrame(end_point));
}

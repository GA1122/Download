void RenderFrameImpl::ExtractSmartClipData(
    const gfx::Rect& rect,
    ExtractSmartClipDataCallback callback) {
  blink::WebString clip_text;
  blink::WebString clip_html;
  blink::WebRect clip_rect;
  GetWebFrame()->ExtractSmartClipData(rect, clip_text, clip_html, clip_rect);
  std::move(callback).Run(clip_text.Utf16(), clip_html.Utf16(), clip_rect);
}

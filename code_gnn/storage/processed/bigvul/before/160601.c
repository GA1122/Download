void RenderFrameImpl::ExtractSmartClipData(
    const gfx::Rect& rect,
    ExtractSmartClipDataCallback callback) {
#if defined(OS_ANDROID)
  blink::WebString clip_text;
  blink::WebString clip_html;
  GetWebFrame()->ExtractSmartClipData(rect, clip_text, clip_html);
  std::move(callback).Run(clip_text.Utf16(), clip_html.Utf16());
#endif   
}

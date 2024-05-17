blink::WebString RenderFrameImpl::DoNotTrackValue() {
  if (render_view_->renderer_preferences_.enable_do_not_track)
    return WebString::FromUTF8("1");
  return WebString();
}

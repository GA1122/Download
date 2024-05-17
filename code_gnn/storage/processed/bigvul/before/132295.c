void RenderFrameImpl::didChangeName(blink::WebLocalFrame* frame,
                                    const blink::WebString& name) {
  DCHECK(!frame_ || frame_ == frame);

  if (SiteIsolationPolicy::AreCrossProcessFramesPossible() ||
      render_view_->renderer_preferences_.report_frame_name_changes) {
    Send(new FrameHostMsg_DidChangeName(
        routing_id_, base::UTF16ToUTF8(base::StringPiece16(name))));
  }
}

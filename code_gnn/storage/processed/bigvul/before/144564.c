bool WebContentsImpl::HasValidFrameSource() {
  if (!render_frame_message_source_) {
    DCHECK(render_view_message_source_);
    bad_message::ReceivedBadMessage(GetRenderProcessHost(),
                                    bad_message::WC_INVALID_FRAME_SOURCE);
    return false;
  }

  return true;
}

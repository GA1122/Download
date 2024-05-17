void RenderMessageFilter::OnMediaLogEvent(const media::MediaLogEvent& event) {
  if (media_observer_)
    media_observer_->OnMediaEvent(render_process_id_, event);
}

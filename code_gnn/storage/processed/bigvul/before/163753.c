void DelegatedFrameHost::AttemptFrameSubscriberCapture(
    const gfx::Rect& damage_rect) {
  if (!frame_subscriber() || !CanCopyFromCompositingSurface())
    return;

  const base::TimeTicks now = tick_clock_->NowTicks();
  base::TimeTicks present_time;
  if (vsync_interval_ <= base::TimeDelta()) {
    present_time = now;
  } else {
    const int64_t intervals_elapsed = (now - vsync_timebase_) / vsync_interval_;
    present_time = vsync_timebase_ + (intervals_elapsed + 1) * vsync_interval_;
  }

  scoped_refptr<media::VideoFrame> frame;
  RenderWidgetHostViewFrameSubscriber::DeliverFrameCallback callback;
  if (!frame_subscriber()->ShouldCaptureFrame(damage_rect, present_time, &frame,
                                              &callback))
    return;

  scoped_refptr<OwnedMailbox> subscriber_texture;
  if (!idle_frame_subscriber_textures_.empty()) {
    subscriber_texture = idle_frame_subscriber_textures_.back();
    idle_frame_subscriber_textures_.pop_back();
  } else if (viz::GLHelper* helper =
                 ImageTransportFactory::GetInstance()->GetGLHelper()) {
    subscriber_texture = new OwnedMailbox(helper);
  }

  std::unique_ptr<viz::CopyOutputRequest> request =
      viz::CopyOutputRequest::CreateRequest(base::BindOnce(
          &DelegatedFrameHost::CopyFromCompositingSurfaceHasResultForVideo,
          AsWeakPtr(), subscriber_texture, frame,
          base::Bind(callback, present_time)));
  request->set_source(frame_subscriber()->GetSourceIdForCopyRequest());
  if (subscriber_texture.get()) {
    request->SetTextureMailbox(viz::TextureMailbox(
        subscriber_texture->mailbox(), subscriber_texture->sync_token(),
        subscriber_texture->target()));
  }

  if (has_frame_ && request_copy_of_output_callback_for_testing_.is_null()) {
    support_->RequestCopyOfSurface(std::move(request));
  } else {
    RequestCopyOfOutput(std::move(request));
  }
}

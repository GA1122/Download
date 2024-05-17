void WebMediaPlayerMS::OnFirstFrameReceived(media::VideoRotation video_rotation,
                                            bool is_opaque) {
  DVLOG(1) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  if (surface_layer_for_video_enabled_) {
    DCHECK(!bridge_);

    bridge_ = std::move(create_bridge_callback_)
                  .Run(this, compositor_->GetUpdateSubmissionStateCallback());
    bridge_->CreateSurfaceLayer();
    bridge_->SetContentsOpaque(opaque_);

    compositor_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(
            &WebMediaPlayerMSCompositor::EnableSubmission, compositor_,
            bridge_->GetSurfaceId(), video_rotation, IsInPictureInPicture(),
            opaque_,
            media::BindToCurrentLoop(base::BindRepeating(
                &WebMediaPlayerMS::OnFrameSinkDestroyed, AsWeakPtr()))));
  }

  SetReadyState(WebMediaPlayer::kReadyStateHaveMetadata);
  SetReadyState(WebMediaPlayer::kReadyStateHaveEnoughData);

  OnRotationChanged(video_rotation, is_opaque);
}

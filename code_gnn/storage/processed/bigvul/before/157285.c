void WebMediaPlayerImpl::OnVideoOpacityChange(bool opaque) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK_NE(ready_state_, WebMediaPlayer::kReadyStateHaveNothing);

  opaque_ = opaque;
  if (!surface_layer_for_video_enabled_) {
    if (video_layer_)
      video_layer_->SetContentsOpaque(opaque_);
  } else if (bridge_->GetCcLayer()) {
    bridge_->SetContentsOpaque(opaque_);
    vfc_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&VideoFrameCompositor::UpdateIsOpaque,
                       base::Unretained(compositor_.get()), opaque_));
  }
}

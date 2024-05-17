void WebMediaPlayerMS::OnOpacityChanged(bool is_opaque) {
  DVLOG(1) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  opaque_ = is_opaque;

  if (!bridge_) {
    video_layer_->SetContentsOpaque(opaque_);
  } else {
    DCHECK(bridge_);

    bridge_->SetContentsOpaque(opaque_);
    compositor_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&WebMediaPlayerMSCompositor::UpdateIsOpaque,
                                  compositor_, opaque_));
  }
}

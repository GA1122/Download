void WebMediaPlayerMS::OnFrameClosed() {
  DCHECK(thread_checker_.CalledOnValidThread());

#if defined(OS_ANDROID)
  if (!paused_) {
    Pause();
    should_play_upon_shown_ = true;
  }

  delegate_->PlayerGone(delegate_id_);
#endif   

  if (frame_deliverer_) {
    io_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&FrameDeliverer::SetRenderFrameSuspended,
                       base::Unretained(frame_deliverer_.get()), true));
  }
}

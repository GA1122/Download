void WebMediaPlayerMS::OnFrameHidden() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (frame_deliverer_) {
    io_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&FrameDeliverer::SetRenderFrameSuspended,
                       base::Unretained(frame_deliverer_.get()), true));
  }

#if defined(OS_ANDROID)
  if (!paused_)
    compositor_->ReplaceCurrentFrameWithACopy();
#endif   
}

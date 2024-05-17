void WebMediaPlayerMS::OnFrameShown() {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (frame_deliverer_) {
    io_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&FrameDeliverer::SetRenderFrameSuspended,
                       base::Unretained(frame_deliverer_.get()), false));
  }

#if defined(OS_ANDROID)
  if (should_play_upon_shown_)
    Play();
#endif   
}

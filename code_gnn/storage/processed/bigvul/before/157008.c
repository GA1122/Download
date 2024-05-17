  void SetRenderFrameSuspended(bool render_frame_suspended) {
    DCHECK(io_thread_checker_.CalledOnValidThread());
    render_frame_suspended_ = render_frame_suspended;
  }

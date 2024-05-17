PepperVideoRenderer2D::~PepperVideoRenderer2D() {
  base::WaitableEvent done_event(true, false);
  software_video_renderer_->RequestReturnBuffers(
      base::Bind(&base::WaitableEvent::Signal, base::Unretained(&done_event)));
  done_event.Wait();

  merge_buffer_ = nullptr;
  while (!buffers_.empty()) {
    FreeBuffer(buffers_.front());
  }
}

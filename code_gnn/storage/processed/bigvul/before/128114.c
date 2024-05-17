void SynchronousCompositorOutputSurface::SwapBuffers(
    cc::CompositorFrame* frame) {
  DCHECK(CalledOnValidThread());
  frame_holder_.reset(new cc::CompositorFrame);
  frame->AssignTo(frame_holder_.get());
  client_->DidSwapBuffers();
}

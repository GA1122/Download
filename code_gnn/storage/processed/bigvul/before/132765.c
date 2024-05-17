 void PepperVideoRenderer2D::OnSessionConfig(
     const protocol::SessionConfig& config) {
   DCHECK(CalledOnValidThread());

  software_video_renderer_->OnSessionConfig(config);
  AllocateBuffers();
}

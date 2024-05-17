ChromotingStats* PepperVideoRenderer2D::GetStats() {
  DCHECK(CalledOnValidThread());

  return software_video_renderer_->GetStats();
}

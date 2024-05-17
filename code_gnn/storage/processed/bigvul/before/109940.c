void GpuVideoDecodeAccelerator::OnReset() {
  DCHECK(video_decode_accelerator_.get());
  video_decode_accelerator_->Reset();
}

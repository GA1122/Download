void GpuVideoDecodeAccelerator::OnFlush() {
  DCHECK(video_decode_accelerator_.get());
  video_decode_accelerator_->Flush();
}

void GpuVideoDecodeAccelerator::OnDestroy() {
  DCHECK(video_decode_accelerator_.get());
  video_decode_accelerator_.release()->Destroy();
}

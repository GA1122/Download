GpuVideoDecodeAccelerator::~GpuVideoDecodeAccelerator() {
  if (video_decode_accelerator_)
    video_decode_accelerator_->Destroy();
}

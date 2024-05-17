void GpuVideoDecodeAccelerator::OnWillDestroyStub(GpuCommandBufferStub* stub) {
  DCHECK_EQ(stub, stub_.get());
  if (video_decode_accelerator_.get())
    video_decode_accelerator_.release()->Destroy();
  if (stub_) {
    stub_->RemoveDestructionObserver(this);
    stub_.reset();
  }
}

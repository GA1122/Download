void DXVAVideoDecodeAccelerator::Invalidate() {
  if (state_ == kUninitialized)
    return;
  ClearState();
  decoder_.Release();
  MFShutdown();
  state_ = kUninitialized;
}

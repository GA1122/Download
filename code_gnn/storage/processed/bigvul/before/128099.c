void TestSynchronousCompositor::SetHardwareFrame(
    scoped_ptr<cc::CompositorFrame> frame) {
  hardware_frame_ = frame.Pass();
}

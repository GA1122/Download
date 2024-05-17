bool TestTarget::AcceleratorPressed(const Accelerator& accelerator) {
  ++accelerator_pressed_count_;
  return true;
}

int Instance::GetScaled(int x) const {
  return static_cast<int>(x * device_scale_);
}
